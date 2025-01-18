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
    int semIdStacja = semget(keyStacja, 1, 0);
    if (semIdStacja == -1) blad("turysta semget stacja");

    int semIdKasjer = semget(keyKasjer, 1, 0);
    if (semIdKasjer == -1) blad("turysta semget kasjer");

    //dolaczanie do kolejki komunikatow
    int msgIdKasjer = msgget(keyKasjer, 0);
    if (msgIdKasjer == -1) blad("turysta msgget kasjer");


    sem_P(semIdStacja);
    bool endSim = infoStacja->koniecSymulacji;
    sem_V(semIdStacja);

    if (endSim){
        cout << "[Turysta #"<< (tId+1) <<"] KONIEC" << endl;
        return 0;
    }

    sleep(rand()%20 + 1);

    sem_P(semIdKasjer);
    //wysylamy prosbe do kasjera
    msgKasjer req;
    req.mtype = 1;
    req.liczbaZjazdow = 0;
    if (msgsnd(msgIdKasjer, &req, sizeof(req) - sizeof(long), 0) == -1) {
        blad("[Turysta] msgsnd kasjer error");
        return 1;
    }
    cout << "[Turysta #"<< (tId+1) <<"] Wyslano prosbe o karnet do kasjera" << endl;

    //czekamy na odpowiedz
    msgKasjer resp;
    while (true){
        ssize_t tmp = msgrcv(msgIdKasjer, &resp, sizeof(resp) - sizeof(long), 2, IPC_NOWAIT);
        if (tmp >=0){
            break;
        }
        else if (errno == ENOMSG) {
            sem_P(semIdStacja);
            bool endSim = infoStacja->koniecSymulacji;
            sem_V(semIdStacja);

            if (endSim) {
                sem_V(semIdKasjer);
                cout << "[Turysta #"<< (tId+1) <<"] KONIEC" << endl;
                shmdt(infoStacja);
                return 0;
            }

            continue;
        }
        else if (errno == EINTR) {
            continue;
        }
        else {
            blad("[Turysta] msgrcv error");
            break;
        }
    }

    sem_V(semIdKasjer);
    cout << "[Turysta #"<< (tId+1) <<"] Otrzymano karnet na " << resp.liczbaZjazdow << " zjazdow" << endl;

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
