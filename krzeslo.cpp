#include "common.h"

void sigusrObsluga(int){}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "[krzeslo] Podaj id krzesla w argumencie!\n";
        return 1;
    }
    int kIdx = atoi(argv[1]);

    signal(SIGUSR1,sigusrObsluga);

    //klucz ipc
    key_t key = ftok(SCIEZKA_KLUCZA, KLUCZ_PROJ);
    if (key == -1) blad("krzeslo ftok");

    //dolaczanie do pamieci dzielonej
    int shmId = shmget(key, sizeof(StacjaInfo), 0);
    if (shmId == -1) blad("krzeslo shmget");
    StacjaInfo* info = (StacjaInfo*)shmat(shmId, nullptr, 0);
    if (info == (void*)-1) blad("krzeslo shmat");

    //podlaczanie do semafora
    int semId = semget(key, 1, 0);
    if (semId == -1) blad("krzeslo semget");

    //dolaczanie do kolejki komunikatow
    int msgId = msgget(key, 0);
    if (msgId == -1) blad("krzeslo msgget");

    //cout << "[Krzeslo #" << (kIdx+1) << "] START" << endl;

    while(true){

        sem_P(semId);
        bool endSim = info->koniecSymulacji && info->liczbaNarciarzyWKolejce==0;
        sem_V(semId);

        if (endSim){
            cout << "[Krzeslo #" << (kIdx + 1) << "] Koniec" << endl;
            break;
        }

        //oczekiwanie na start jazdy
        Komunikat msg;
        if (msgrcv(msgId, &msg, sizeof(Komunikat)-sizeof(long), 100 + kIdx, 0) == -1) {
            if (errno == EINTR) {
                sem_P(semId);
                endSim = info->koniecSymulacji;
                sem_V(semId);

                if (endSim) {
                    cout << "[Krzeslo #" << (kIdx + 1) << "] Koniec (sygnal)" <<endl;
                    break;
                }
                continue;
            } else {
                perror("[krzeslo] msgrcv start error");
                sleep(1);
                continue;
            }
        }

        int ileOsob = msg.liczbaOsob;
        cout << "[Krzeslo #" << (kIdx+1) << "] Ruszam w gore z " << ileOsob << " osobami" << endl;


        sleep(40);

        //komunikat na górze
        Komunikat msg2;
        msg2.mtype = 200 + kIdx;
        msg2.nrKrzesla = kIdx;
        msg2.liczbaOsob= ileOsob;
        msgsnd(msgId, &msg2, sizeof(Komunikat)-sizeof(long), 0);

        //oczekiwanie na wracaj
        Komunikat msg3;
        if(msgrcv(msgId, &msg3, sizeof(Komunikat)-sizeof(long), 300+kIdx, 0) == -1){
            perror("[krzeslo] msgrcv return");
            sleep(1);
            continue;
        }
        cout << "[Krzeslo #" << (kIdx+1) << "] Wracam na dol" << endl;

        sem_P(semId);
        info->stanKrzesla[kIdx] = 2;
        info->ileOsobNaKrzesle[kIdx] = 0;
        sem_V(semId);

        sleep(40);

        sem_P(semId);
        info->stanKrzesla[kIdx] = 0;
        sem_V(semId);

        cout << "[Krzeslo #" << (kIdx+1) << "] Jestem wolne na dole" << endl;
    }

    //odlaczenie pamieci
    shmdt(info);
    return 0;
}
