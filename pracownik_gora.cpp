#include "common.h"

int main()
{
    //klucze ipc
    key_t keyStacja = ftok(SCIEZKA_KLUCZA_STACJA, KLUCZ_PROJ_STACJA);
    if (keyStacja == -1) blad("pracownik_dol ftok stacja");

    key_t keyWyciag = ftok(SCIEZKA_KLUCZA_WYCIAG, KLUCZ_PROJ_WYCIAG);
    if (keyWyciag == -1) blad("pracownik_dol ftok wyciag");

    //dolaczanie do pamieci dzielonej
    int shmIdStacja = shmget(keyStacja, sizeof(StacjaInfo), 0);
    if (shmIdStacja == -1) blad("pracownik_gora shmget stacja");
    StacjaInfo* infoStacja = (StacjaInfo*)shmat(shmIdStacja, nullptr, 0);
    if (infoStacja == (void*)-1) blad("pracownik_gora shmat stacja");

    int shmIdWyciag = shmget(keyWyciag, sizeof(WyciagInfo), 0);
    if (shmIdWyciag == -1) blad("pracownik_gora shmget wyciag");
    WyciagInfo* infoWyciag = (WyciagInfo*)shmat(shmIdWyciag, nullptr, 0);
    if (infoWyciag == (void*)-1) blad("pracownik_gora shmat wyciag");

    //podlaczanie do semafora
    int semIdStacja = semget(keyStacja, 1, 0);
    if (semIdStacja == -1) blad("pracownik_gora semget stacja");

    int semIdWyciag = semget(keyWyciag, 1, 0);
    if (semIdWyciag == -1) blad("pracownik_gora semget wyciag");

    //dolaczanie do kolejki komunikatow
    int msgIdWyciag = msgget(keyWyciag, 0);
    if (msgIdWyciag == -1) blad("pracownik_gora msgget wyciag");

    cout << "[Pracownik Gorna Stacja] START" << endl;

    while(true) {

        sem_P(semIdStacja);
        sem_P(semIdWyciag);
        bool endSim = infoStacja->koniecSymulacji && infoWyciag->krzeslaWTrasie==0;
        sem_V(semIdStacja);
        sem_V(semIdWyciag);

        if (endSim){
            cout << "[Pracownik Gorna Stacja] Koniec" << endl;
            break;
        }
        //oczekiwanie na komunikat krzesla
        Komunikat msg;
        if (msgrcv(msgIdWyciag, &msg, sizeof(Komunikat)-sizeof(long), 0, 0) == -1) {
            blad("[Pracownik Gorna Stacja] msgrcv");
            sleep(1);
            continue;
        }

        long baseType = msg.mtype - 200;
        if (baseType >= 0 && baseType < 80) {
            int kIdx = msg.nrKrzesla;
            int ile = msg.liczbaOsob;

            sem_P(semIdWyciag);
            infoWyciag->krzeslaWTrasie--;
            infoWyciag->liczbaNarciarzyWTrasie -= msg.liczbaOsob;
            cout << "[Pracownik Gorna Stacja] Krzeslo #" << (kIdx+1) << " dotarło z " << ile << " osobami. wTrasie=" << infoWyciag->krzeslaWTrasie << endl;
            sem_V(semIdWyciag);

            //odeslanie krzesla
            Komunikat msg2;
            msg2.mtype = 300 + kIdx;
            msg2.nrKrzesla = kIdx;
            msg2.liczbaOsob= 0;
            msgsnd(msgIdWyciag, &msg2, sizeof(Komunikat)-sizeof(long), 0);

        }
    }

    //odlaczenie pamieci
    shmdt(infoStacja);
    shmdt(infoWyciag);
    return 0;
}
