#include "common.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "[krzeslo] Podaj id krzesla w argumencie!\n";
        return 1;
    }
    int kIdx = atoi(argv[1]);

    key_t key = ftok(SCIEZKA_KLUCZA, KLUCZ_PROJ);
    if (key == -1) blad("krzeslo ftok");

    int shmId = shmget(key, sizeof(StacjaInfo), 0);
    if (shmId == -1) blad("krzeslo shmget");
    StacjaInfo* info = (StacjaInfo*)shmat(shmId, nullptr, 0);
    if (info == (void*)-1) blad("krzeslo shmat");

    int semId = semget(key, 1, 0);
    if (semId == -1) blad("krzeslo semget");

    int msgId = msgget(key, 0);
    if (msgId == -1) blad("krzeslo msgget");

    //cout << "[Krzeslo #" << (kIdx+1) << "] START\n";

    while(true){
        Komunikat msg;
        if (msgrcv(msgId, &msg, sizeof(Komunikat)-sizeof(long), 100+kIdx, 0) == -1) {
            perror("[krzeslo] msgrcv start");
            sleep(1);
            continue;
        }
        int ileOsob = msg.liczbaOsob;
        cout << "[Krzeslo #" << (kIdx+1) << "] Ruszam w gore z "
                  << ileOsob << " osobami.\n";

        sleep(40);

        Komunikat msg2;
        msg2.mtype = 200 + kIdx;
        msg2.nrKrzesla = kIdx;
        msg2.liczbaOsob= ileOsob;
        msgsnd(msgId, &msg2, sizeof(Komunikat)-sizeof(long), 0);

        Komunikat msg3;
        if(msgrcv(msgId, &msg3, sizeof(Komunikat)-sizeof(long), 300+kIdx, 0) == -1){
            perror("[krzeslo] msgrcv return");
            sleep(1);
            continue;
        }
        cout << "[Krzeslo #" << (kIdx+1) << "] Wracam na dol.\n";

        sleep(40);

        sem_P(semId);
        info->stanKrzesla[kIdx] = 0;
        info->ileOsobNaKrzesle[kIdx] = 0;
        sem_V(semId);

        cout << "[Krzeslo #" << (kIdx+1) << "] Jestem wolne na dole.\n";
    }

    return 0;
}
