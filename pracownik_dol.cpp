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

    cout << "[Pracownik Dolna Stacja] START" << endl;

    while(true) {
        sleep(1);

        sem_P(semIdStacja);
        sem_P(semIdBramki);
        if (infoStacja->koniecSymulacji && infoBramki->liczbaNarciarzyWKolejce == 0)
        {
            sem_V(semIdStacja);
            sem_V(semIdBramki);
            cout << "[Pracownik Dolna Stacja] Koniec" << endl;
            break;
        }
        sem_V(semIdStacja);
        sem_V(semIdWyciag);
        if (infoBramki->liczbaNarciarzyWKolejce == 0 && infoWyciag->liczbaNarciarzyWTrasie == 0) {
            sem_V(semIdWyciag);
            sem_V(semIdBramki);
            sleep(1);
            continue;
        }

        if (infoWyciag->krzeslaWTrasie < 40) {
            int wolneIdx = -1;
            for (int i=0; i<80; i++){
                if (infoWyciag->stanKrzesla[i] == 0) {
                    wolneIdx = i;
                    break;
                }
            }
            if (wolneIdx != -1) {
                int ileDoZabrania = min(3, infoBramki->liczbaNarciarzyWKolejce);
                infoWyciag->stanKrzesla[wolneIdx] = 1;
                infoWyciag->ileOsobNaKrzesle[wolneIdx] = ileDoZabrania;
                infoBramki->liczbaNarciarzyWKolejce -= ileDoZabrania;
                infoWyciag->liczbaNarciarzyWTrasie += ileDoZabrania;
                infoWyciag->krzeslaWTrasie++;
                cout << "[Pracownik Dolna Stacja] Wypuszczam krzeslo #" << (wolneIdx+1) << " z " << ileDoZabrania << " osobami. wTrasie=" << infoWyciag->krzeslaWTrasie << endl;
                sem_V(semIdWyciag);
                sem_V(semIdBramki);
                //wyslanie start do krzesla
                Komunikat msg;
                msg.mtype = 100 + wolneIdx;
                msg.nrKrzesla = wolneIdx;
                msg.liczbaOsob= ileDoZabrania;
                if (msgsnd(msgIdWyciag, &msg, sizeof(Komunikat)-sizeof(long), 0) == -1) {
                    blad("[Pracownik Dolna Stacja] msgsnd krzeslo error");
                }
                continue;
            }
        }
        sem_V(semIdWyciag);
        sem_V(semIdBramki);
    }

    //odlaczenie pamieci
    shmdt(infoStacja);
    shmdt(infoWyciag);
    shmdt(infoBramki);
    return 0;
}
