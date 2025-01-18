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

    int msgIdNarciarz = msgget(keyWyciag, 0);
    if (msgIdNarciarz == -1) blad("pracownik_gora msgget narciarz");

    cout << "[Pracownik Gorna Stacja] START" << endl;

    while(true) {

        sem_P(semIdStacja);
        sem_P(semIdWyciag);
        bool endSim = infoStacja->koniecSymulacji && infoWyciag->krzeslaWTrasie==0;
        sem_V(semIdStacja);
        sem_V(semIdWyciag);

        if (endSim){
            cout << "[Pracownik Gorna Stacja] KONIEC" << endl;
            break;
        }
        //oczekiwanie na komunikat krzesla
        msgWyciag msg1;
        if (msgrcv(msgIdWyciag, &msg1, sizeof(msgWyciag)-sizeof(long), 0, 0) == -1) {
            blad("[Pracownik Gorna Stacja] msgrcv krzeslo error");
            sleep(1);
            continue;
        }

        long baseType = msg1.mtype - 200;
        if (baseType >= 0 && baseType < 80) {
            int kId = msg1.nrKrzesla;
            int ile = msg1.liczbaOsob;

            sem_P(semIdWyciag);
            infoWyciag->krzeslaWTrasie--;
            infoWyciag->liczbaNarciarzyWTrasie -= ile;
            cout << "[Pracownik Gorna Stacja] Krzeslo #" << (kId+1) << " dotarło z " << ile << " osobami. wTrasie=" << infoWyciag->krzeslaWTrasie << endl;
            sem_V(semIdWyciag);

            //wysiadanie narciarzy
            for (int i=0;i<ile;i++){
                msgNarciarz msg2;
                msg2.mtype = 1000+msg1.idNarciarzyNaKrzesle[i];
                msg2.narciarzId = msg1.idNarciarzyNaKrzesle[i];
                if(msgsnd(msgIdNarciarz, &msg2, sizeof(msgNarciarz)-sizeof(long), 0) ==-1){
                    blad("[Pracownik Gorna Stacja] msgsnd narciarz error");
                    return 1;
                }
                cout << "[Pracownik Gorna Stacja] Narciarz #" << msg1.idNarciarzyNaKrzesle[i] +1 << " wysiada" << endl;
            }

            //odeslanie krzesla
            msgWyciag msg3;
            msg3.mtype = 300 + kId;
            msg3.nrKrzesla = kId;
            msg3.liczbaOsob= 0;
            memset(msg3.idNarciarzyNaKrzesle, 0, sizeof(msg3.idNarciarzyNaKrzesle));
            if (msgsnd(msgIdWyciag, &msg3, sizeof(msgWyciag)-sizeof(long), 0)==-1){
                blad("[Pracownik Gorna Stacja] msgsnd krzeslo error");
                return 1;
            }

        }
    }

    //odlaczenie pamieci
    shmdt(infoStacja);
    shmdt(infoWyciag);
    return 0;
}
