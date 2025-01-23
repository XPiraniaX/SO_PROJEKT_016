#include "common.h"

int main()
{
    //klucze ipc
    key_t keyWyciag = ftok(SCIEZKA_KLUCZA_WYCIAG, KLUCZ_PROJ_WYCIAG);
    if (keyWyciag == -1) blad("pracownik_dol ftok wyciag");

    key_t keyBramki = ftok(SCIEZKA_KLUCZA_BRAMKI, KLUCZ_PROJ_BRAMKI);
    if (keyBramki == -1) blad("pracownik_dol ftok bramki");

    //dolaczanie do pamieci dzielonej
    int shmIdWyciag = shmget(keyWyciag, sizeof(WyciagInfo), 0);
    if (shmIdWyciag == -1) blad("pracownik_dol shmget wyciag");
    WyciagInfo* infoWyciag = (WyciagInfo*)shmat(shmIdWyciag, nullptr, 0);
    if (infoWyciag == (void*)-1) blad("pracownik_dol shmat wyciag");

    int shmIdBramki = shmget(keyBramki, sizeof(BramkiInfo), 0);
    if (shmIdBramki == -1) blad("pracownik_dol shmget bramki");
    BramkiInfo* infoBramki = (BramkiInfo*)shmat(shmIdBramki, nullptr, 0);
    if (infoBramki == (void*)-1) blad("pracownik_dol shmat bramki");

    //podlaczanie do semafora
    int semIdWyciag = semget(keyWyciag, 1, 0);
    if (semIdWyciag == -1) blad("pracownik_dol semget wyciag");

    int semIdBramki = semget(keyBramki, 1, 0);
    if (semIdBramki == -1) blad("pracownik_dol semget bramki");

    //dolaczanie do kolejki komunikatow
    int msgIdWyciag = msgget(keyWyciag, 0);
    if (msgIdWyciag == -1) blad("pracownik_dol msgget wyciag");

    bool zamykanie = false;
    bool kolejkaPusta = false;
    cout << "\033[32m[Pracownik Dolna Stacja] START\033[0m" << endl;

    while(true) {

        sleep(1);

        sem_P(semIdWyciag);
        sem_P(semIdBramki);

        msgWyciag koniec;
        if (msgrcv(msgIdWyciag, &koniec, sizeof(koniec)-sizeof(long), 99999, IPC_NOWAIT) != -1){
            zamykanie = true;
        }
        else if (errno ==ENOMSG){}
        else{
            blad("[Pracownik Dolna Stacja] msgrcv init error");
        }

        if (infoWyciag->krzeslaWTrasie < 40) {
            int wolnekId = -1;
            for (int i=0; i<80; i++){
                if (infoWyciag->stanKrzesla[i] == 0) {
                    wolnekId = i;
                    break;
                }
            }
            if (wolnekId != -1) {

                int wolneMiejsca =3;
                vector<Narciarz> IdNarciarzy;
                while (wolneMiejsca > 0 && infoBramki->liczbaNarciarzyWKolejce > 0){

                    int ileOsob = infoBramki->kolejka[infoBramki->przod].liczbadzieci+1;

                    if (ileOsob <= wolneMiejsca){
                        Narciarz narciarz = popNarciarz(infoBramki);
                        if (narciarz.narciarzId==-1)break;

                        for (int i=0;i<ileOsob;i++){
                            IdNarciarzy.push_back(narciarz);
                        }

                        wolneMiejsca-=ileOsob;

                        if (narciarz.liczbadzieci > 0){
                            cout << "\033[32m[Pracownik Dolna Stacja] Laduje narciarza #" << (narciarz.narciarzId+1) <<" z dziecmi w liczbie "<< narciarz.liczbadzieci <<"\033[0m"<< endl;
                        }
                        else{
                            cout << "\033[32m[Pracownik Dolna Stacja] Laduje narciarza #" << (narciarz.narciarzId+1) <<"\033[0m"<< endl;
                        }
                    }
                    else{
                        break;
                    }
                }

                infoWyciag->stanKrzesla[wolnekId] = 1;
                infoWyciag->ileOsobNaKrzesle[wolnekId] = IdNarciarzy.size();
                infoWyciag->liczbaNarciarzyWTrasie += IdNarciarzy.size();
                infoWyciag->krzeslaWTrasie++;
                if (zamykanie){
                    if (czyKolejkaPusta(infoBramki)&& infoWyciag->liczbaNarciarzyWTrasie == 0){
                        sem_V(semIdBramki);
                        sem_V(semIdWyciag);
                        msgWyciag koniecPracownikGora;
                        koniecPracownikGora.mtype=100000;
                        if (msgsnd(msgIdWyciag, &koniecPracownikGora, sizeof(koniecPracownikGora) - sizeof(long), 0) == -1) {
                            blad("[INIT] msgsnd pracownicy error");
                        }
                        sleep(5);
                        break;
                    }
                }
                cout << "\033[32m[Pracownik Dolna Stacja] Wypuszczam krzeslo #" << (wolnekId+1) << " z " << IdNarciarzy.size() << " osobami. wTrasie=" << infoWyciag->krzeslaWTrasie <<"\033[0m"<< endl;
                sem_V(semIdBramki);
                sem_V(semIdWyciag);
                //wyslanie start do krzesla
                msgWyciag msg;
                msg.mtype = 100 + wolnekId;
                msg.nrKrzesla = wolnekId;
                msg.liczbaOsob= IdNarciarzy.size();
                for (int i=0; i<IdNarciarzy.size(); i++){
                    msg.idNarciarzyNaKrzesle[i] = IdNarciarzy[i].narciarzId;
                }
                if (msgsnd(msgIdWyciag, &msg, sizeof(msgWyciag)-sizeof(long), 0) == -1) {
                    blad("[Pracownik Dolna Stacja] msgsnd krzeslo error");
                }
            }
            else{
                sem_V(semIdBramki);
                sem_V(semIdWyciag);
                continue;
            }

        }
        else{
            sem_V(semIdBramki);
            sem_V(semIdWyciag);
        }
    }
    cout << "\033[32m[Pracownik Dolna Stacja] Wszyscy sa na gorze i kolejka pusta KONIEC\033[0m" << endl;
    //odlaczenie pamieci
    shmdt(infoWyciag);
    shmdt(infoBramki);
    return 0;
}