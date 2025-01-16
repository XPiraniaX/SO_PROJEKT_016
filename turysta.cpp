#include "common.h"

int main()
{
    //klucze ipc
    key_t keyKasjer = ftok(SCIEZKA_KLUCZA_KASJER, KLUCZ_PROJ_KASJER);
    if (keyKasjer == -1) blad("kasjer ftok kasjer");

    //podlaczanie do semafora
    int semIdKasjer = semget(keyKasjer, 1, 0);
    if (semIdKasjer == -1) blad("kasjer semget kasjer");

    //dolaczanie do kolejki komunikatow
    int msgIdKasjer = msgget(keyKasjer, 0);
    if (msgIdKasjer == -1) blad("kasjer msgget kasjer");


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
    cout << "[Turysta] Wyslano prosbe o karnet do kasjera" << endl;

    //czekamy na odpowiedz
    msgKasjer resp;
    if (msgrcv(msgIdKasjer, &resp, sizeof(resp) - sizeof(long), 2, 0) == -1) {
        blad("[Turysta] msgrcv kasjer error");
        return 1;
    }
    sem_V(semIdKasjer);
    cout << "[Turysta] Otrzymano karnet na " << resp.liczbaZjazdow << " zjazdow" << endl;


    pid_t pn = fork();
    if (pn == 0) {
        char buf[16];
        sprintf(buf, "%d", resp.liczbaZjazdow);
        execlp("./narciarz", "narciarz", buf, (char*)nullptr);
        blad("execlp narciarz");
    } else if (pn > 0) {
        cout << "[Turysta] Tworze narciarza PID=" << pn << " z " << resp.liczbaZjazdow << " zjazdami" << endl;
    }

    cout << "[Turysta] KONIEC" << endl;
    return 0;
}
