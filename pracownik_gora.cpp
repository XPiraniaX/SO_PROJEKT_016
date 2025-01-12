#include "common.h"

int main()
{
    key_t key = ftok(SCIEZKA_KLUCZA, KLUCZ_PROJ);
    if (key == -1) blad("pracownik_gora ftok");

    int shmId = shmget(key, sizeof(StacjaInfo), 0);
    if (shmId == -1) blad("pracownik_gora shmget");
    StacjaInfo* info = (StacjaInfo*)shmat(shmId, nullptr, 0);
    if (info == (void*)-1) blad("pracownik_gora shmat");

    int semId = semget(key, 1, 0);
    if (semId == -1) blad("pracownik_gora semget");

    int msgId = msgget(key, 0);
    if (msgId == -1) blad("pracownik_gora msgget");

    cout << "[Pracownik Gorna Stacja] START\n";

    while(true) {
        Komunikat msg;
        if (msgrcv(msgId, &msg, sizeof(Komunikat)-sizeof(long), 0, 0) == -1) {
            perror("[Pracownik Gorna Stacja] msgrcv");
            sleep(1);
            continue;
        }

        long baseType = msg.mtype - 200;
        if (baseType >= 0 && baseType < 80) {
            int kIdx = msg.nrKrzesla;
            int ile = msg.liczbaOsob;

            sem_P(semId);
            info->krzeslaWTrasie--;
            info->liczbaNarciarzyWTrasie -= msg.liczbaOsob;
            cout << "[Pracownik Gorna Stacja] Krzeslo #" << (kIdx+1)
                      << " dotarło z " << ile << " osobami. wTrasie="
                      << info->krzeslaWTrasie << "\n";
            sem_V(semId);

            Komunikat msg2;
            msg2.mtype = 300 + kIdx;
            msg2.nrKrzesla = kIdx;
            msg2.liczbaOsob= 0;
            msgsnd(msgId, &msg2, sizeof(Komunikat)-sizeof(long), 0);
        }
    }

    return 0;
}
