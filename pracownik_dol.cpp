#include "common.h"

int main()
{
    //klucz ipc
    key_t key = ftok(SCIEZKA_KLUCZA, KLUCZ_PROJ);
    if (key == -1) blad("pracownik_dol ftok");

    //dolaczanie do pamieci dzielonej
    int shmId = shmget(key, sizeof(StacjaInfo), 0);
    if (shmId == -1) blad("pracownik_dol shmget");
    StacjaInfo* info = (StacjaInfo*)shmat(shmId, nullptr, 0);
    if (info == (void*)-1) blad("pracownik_dol shmat");

    //podlaczanie do semafora
    int semId = semget(key, 1, 0);
    if (semId == -1) blad("pracownik_dol semget");

    //dolaczanie do kolejki komunikatow
    int msgId = msgget(key, 0);
    if (msgId == -1) blad("pracownik_dol msgget");

    cout << "[Pracownik Dolna Stacja] START" << endl;

    while(true) {
        sleep(1);

        sem_P(semId);

        if (info->koniecSymulacji && info->liczbaNarciarzyWKolejce == 0)
        {
            sem_V(semId);
            cout << "[Pracownik Dolna Stacja] Koniec" << endl;
            break;
        }


        if (info->liczbaNarciarzyWKolejce == 0 && info->liczbaNarciarzyWTrasie == 0) {
            sem_V(semId);
            sleep(1);
            continue;
        }

        if (info->krzeslaWTrasie < 40) {
            int wolneIdx = -1;
            for (int i=0; i<80; i++){
                if (info->stanKrzesla[i] == 0) {
                    wolneIdx = i;
                    break;
                }
            }
            if (wolneIdx != -1) {
                int ileDoZabrania = min(3, info->liczbaNarciarzyWKolejce);
                info->stanKrzesla[wolneIdx] = 1;
                info->ileOsobNaKrzesle[wolneIdx] = ileDoZabrania;
                info->liczbaNarciarzyWKolejce -= ileDoZabrania;
                info->liczbaNarciarzyWTrasie += ileDoZabrania;
                info->krzeslaWTrasie++;
                cout << "[Pracownik Dolna Stacja] Wypuszczam krzeslo #" << (wolneIdx+1) << " z " << ileDoZabrania << " osobami. wTrasie=" << info->krzeslaWTrasie << endl;

                //wyslanie start do krzesla
                Komunikat msg;
                msg.mtype = 100 + wolneIdx;
                msg.nrKrzesla = wolneIdx;
                msg.liczbaOsob= ileDoZabrania;
                if (msgsnd(msgId, &msg, sizeof(Komunikat)-sizeof(long), 0) == -1) {
                    perror("[Pracownik Dolna Stacja] msgsnd krzeslo error");
                }
            }
        }
        sem_V(semId);
    }

    //odlaczenie pamieci
    shmdt(info);
    return 0;
}
