#include "common.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        blad("[Turysta] Podaj id krzesla w argumencie!");
        return 1;
    }
    int tId = atoi(argv[1]);

    //klucze ipc
    key_t keyStacja = ftok(SCIEZKA_KLUCZA_STACJA, KLUCZ_PROJ_STACJA);
    if (keyStacja == -1) blad("turysta ftok stacja");

    key_t keyKasjer = ftok(SCIEZKA_KLUCZA_KASJER, KLUCZ_PROJ_KASJER);
    if (keyKasjer == -1) blad("turysta ftok kasjer");

    //dolaczanie do pamieci dzielonej
    int shmIdStacja = shmget(keyStacja, sizeof(StacjaInfo), 0);
    if (shmIdStacja == -1) blad("turysta shmget stacja");
    StacjaInfo* infoStacja = (StacjaInfo*)shmat(shmIdStacja, nullptr, 0);
    if (infoStacja == (void*)-1) blad("turysta shmat stacja");

    //podlaczanie do semafora
    int semIdKasjer = semget(keyKasjer, 1, 0);
    if (semIdKasjer == -1) blad("turysta semget kasjera");

    //dolaczanie do kolejki komunikatow
    int msgIdKasjer = msgget(keyKasjer, 0);
    if (msgIdKasjer == -1) blad("turysta msgget kasjer");


    if (infoStacja->koniecSymulacji)
    {
        cout << "[Turysta #"<< (tId+1) <<"] Stacja się zamyka, ide do domu KONIEC" << endl;
        shmdt(infoStacja);
        return 0;
    }

    sleep(rand()%25);
    sem_P(semIdKasjer);
    //wysylamy prosbe do kasjera
    msgKasjer req;
    req.mtype = 1;
    req.liczbaZjazdow = 3;
    while (true)
    {
        if (infoStacja->koniecSymulacji)
        {
            cout << "[Turysta #"<< (tId+1) <<"] Stacja się zamyka, ide do domu KONIEC" << endl;
            shmdt(infoStacja);
            return 0;
        }
        if (msgsnd(msgIdKasjer, &req, sizeof(req) - sizeof(long), IPC_NOWAIT) == -1) {
            if (errno == EAGAIN)
            {
                sleep(1);
                continue;
            }
            blad("[Turysta] msgsnd kasjer error");
            return 1;
        }
        break;
    }

    if (infoStacja->koniecSymulacji)
    {
        cout << "[Turysta #"<< (tId+1) <<"] Stacja się zamyka, ide do domu KONIEC" << endl;
        sem_V(semIdKasjer);
        shmdt(infoStacja);
        return 0;
    }

    cout << "[Turysta #"<< (tId+1) <<"] Wyslano prosbe o karnet do kasjera" << endl;

    //czekamy na odpowiedz
    msgKasjer resp;

    msgrcv(msgIdKasjer, &resp, sizeof(resp) - sizeof(long), 2, 0);

    cout << "[Turysta #"<< (tId+1) <<"] Otrzymano karnet na " << resp.liczbaZjazdow << " zjazdow" << endl;
    sem_V(semIdKasjer);

    sleep(rand()%3);

    pid_t pn = fork();
    if (pn == 0) {
        char buf1[16];
        char buf2[16];
        sprintf(buf1, "%d", resp.liczbaZjazdow);
        sprintf(buf2, "%d", tId);
        execlp("./narciarz", "narciarz", buf1, buf2 ,(char*)nullptr);
        blad("execlp narciarz");
    } else if (pn > 0) {
        cout << "[Turysta #"<< (tId+1) <<"] Zostaje narciarzem z " << resp.liczbaZjazdow << " zjazdami" << endl;
    }

    shmdt(infoStacja);
    return 0;
}
