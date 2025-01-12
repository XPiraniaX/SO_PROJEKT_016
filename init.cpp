#include "common.h"

int main()
{
    key_t key = ftok(SCIEZKA_KLUCZA, KLUCZ_PROJ);
    if(key == -1) blad("init ftok");

    int shmId = shmget(key, sizeof(StacjaInfo), IPC_CREAT | 0600);
    if(shmId == -1) blad("init shmget");

    StacjaInfo* info = (StacjaInfo*)shmat(shmId, nullptr, 0);
    if (info == (void*)-1) blad("init shmat");
    info->krzeslaWTrasie = 0;
    info->liczbaNarciarzyWKolejce = 0;
    for(int i=0; i<80; i++){
        info->stanKrzesla[i] = 0; // wolne
        info->ileOsobNaKrzesle[i] = 0;
    }
    shmdt(info);

    int semId = semget(key, 1, IPC_CREAT | 0600);
    if (semId == -1) blad("init semget");
    semun arg;
    arg.val = 1;
    if (semctl(semId, 0, SETVAL, arg) == -1) blad("init semctl SETVAL");

    int msgId = msgget(key, IPC_CREAT | 0600);
    if (msgId == -1) blad("init msgget");

    cout << "[INIT] Zasoby IPC utworzone.\n";

    pid_t pd = fork();
    if (pd == 0) {
        execlp("./pracownik_dol", "pracownik_dol", nullptr);
        blad("execlp pracownik_dol");
    }
    pid_t pg = fork();
    if (pg == 0) {
        execlp("./pracownik_gora", "pracownik_gora", nullptr);
        blad("execlp pracownik_gora");
    }

    for(int i=0; i<80; i++){
        pid_t pk = fork();
        if(pk == 0) {
            char buf[16];
            sprintf(buf, "%d", i);
            execlp("./krzeslo", "krzeslo", buf, nullptr);
            blad("execlp krzeslo");
        }
    }

    for(int i=0; i<LICZBA_NARCIARZY; i++){
        pid_t pn = fork();
        if(pn == 0){
            execlp("./narciarz", "narciarz", nullptr);
            blad("execlp narciarz");
        }
    }

    for(int i = 0; i < 2 + 80 + LICZBA_NARCIARZY; i++){
        wait(nullptr);
    }

    cout << "[INIT] Usuwam zasoby IPC...\n";
    shmctl(shmId, IPC_RMID, nullptr);
    semctl(semId, 0, IPC_RMID);
    msgctl(msgId, IPC_RMID, nullptr);

    cout << "[INIT] Koniec.\n";
    return 0;
}
