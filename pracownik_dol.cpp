#include "common.h"

int main()
{
    //klucze ipc
    key_t keyStacja = ftok(SCIEZKA_KLUCZA_STACJA, KLUCZ_PROJ_STACJA);
    if (keyStacja == -1) blad("pracownik_dol ftok stacja");

    key_t keyWyciag = ftok(SCIEZKA_KLUCZA_WYCIAG, KLUCZ_PROJ_WYCIAG);
    if (keyWyciag == -1) blad("pracownik_dol ftok wyciag");

    key_t keyBramki = ftok(SCIEZKA_KLUCZA_BRAMKI, KLUCZ_PROJ_BRAMKI);
    if (keyBramki == -1) blad("pracownik_dol ftok bramki");

    //dolaczanie do pamieci dzielonej
    int shmIdStacja = shmget(keyStacja, sizeof(StacjaInfo), 0);
    if (shmIdStacja == -1) blad("pracownik_dol shmget stacja");
    StacjaInfo* infoStacja = (StacjaInfo*)shmat(shmIdStacja, nullptr, 0);
    if (infoStacja == (void*)-1) blad("pracownik_dol shmat stacja");

    int shmIdWyciag = shmget(keyWyciag, sizeof(WyciagInfo), 0);
    if (shmIdWyciag == -1) blad("pracownik_dol shmget wyciag");
    WyciagInfo* infoWyciag = (WyciagInfo*)shmat(shmIdWyciag, nullptr, 0);
    if (infoWyciag == (void*)-1) blad("pracownik_dol shmat wyciag");

    int shmIdBramki = shmget(keyBramki, sizeof(BramkiInfo), 0);
    if (shmIdBramki == -1) blad("pracownik_dol shmget bramki");
    BramkiInfo* infoBramki = (BramkiInfo*)shmat(shmIdBramki, nullptr, 0);
    if (infoBramki == (void*)-1) blad("pracownik_dol shmat bramki");

    //podlaczanie do semafora
    int semIdStacja = semget(keyStacja, 1, 0);
    if (semIdStacja == -1) blad("pracownik_dol semget stacja");

    int semIdWyciag = semget(keyWyciag, 1, 0);
    if (semIdWyciag == -1) blad("pracownik_dol semget wyciag");

    int semIdBramki = semget(keyBramki, 1, 0);
    if (semIdBramki == -1) blad("pracownik_dol semget bramki");

    //dolaczanie do kolejki komunikatow
    int msgIdWyciag = msgget(keyWyciag, 0);
    if (msgIdWyciag == -1) blad("pracownik_dol msgget wyciag");

    cout << "\033[32m[Pracownik Dolna Stacja] START\033[0m" << endl;

    while(true) {
        sleep(1);

        //sem_P(semIdStacja);
        sem_P(semIdWyciag);
        sem_P(semIdBramki);
        /*if (infoStacja->koniecSymulacji && infoBramki->liczbaNarciarzyWKolejce == 0 && infoWyciag->liczbaNarciarzyWTrasie==0)
        {
            sem_V(semIdBramki);
            sem_V(semIdWyciag);
            sem_V(semIdStacja);
            cout << "\033[32m[Pracownik Dolna Stacja] KONIEC\033[0m" << endl;
            break;
        }*/


        if (infoWyciag->krzeslaWTrasie < 40) {
            int wolnekId = -1;
            for (int i=0; i<80; i++){
                if (infoWyciag->stanKrzesla[i] == 0) {
                    wolnekId = i;
                    break;
                }
            }
            if (wolnekId != -1) {

                int ileDoZabrania = min(3, infoBramki->liczbaNarciarzyWKolejce);
                vector<int> IdNarciarzy;
                for (int i=0; i<ileDoZabrania; i++){
                    int nId = popNarciarz(infoBramki);
                    if (nId == -1) break;
                    IdNarciarzy.push_back(nId);
                }

                infoWyciag->stanKrzesla[wolnekId] = 1;
                infoWyciag->ileOsobNaKrzesle[wolnekId] = IdNarciarzy.size();
                infoWyciag->liczbaNarciarzyWTrasie += IdNarciarzy.size();
                infoWyciag->krzeslaWTrasie++;
                cout << "\033[32m[Pracownik Dolna Stacja] Wypuszczam krzeslo #" << (wolnekId+1) << " z " << IdNarciarzy.size() << " osobami. wTrasie=" << infoWyciag->krzeslaWTrasie <<"\033[0m"<< endl;
                sem_V(semIdBramki);
                sem_V(semIdWyciag);
                //wyslanie start do krzesla
                msgWyciag msg2;
                msg2.mtype = 100 + wolnekId;
                msg2.nrKrzesla = wolnekId;
                msg2.liczbaOsob= IdNarciarzy.size();
                for (int i=0; i<IdNarciarzy.size(); i++){
                    msg2.idNarciarzyNaKrzesle[i] = IdNarciarzy[i];
                }
                if (msgsnd(msgIdWyciag, &msg2, sizeof(msgWyciag)-sizeof(long), 0) == -1) {
                    blad("[Pracownik Dolna Stacja] msgsnd krzeslo error");
                }
                continue;
            }
        }
        else{
            sem_V(semIdBramki);
            sem_V(semIdWyciag);
        }
    }

    //odlaczenie pamieci
    shmdt(infoStacja);
    shmdt(infoWyciag);
    shmdt(infoBramki);
    return 0;
}