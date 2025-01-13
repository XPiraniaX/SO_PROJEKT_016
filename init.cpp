#include "common.h"

static StacjaInfo* g_info = nullptr;
static int g_semId=-1;

void sigintObsluga(int) {
    if (g_info) {
        sem_P(g_semId);
        g_info->koniecSymulacji = true;
        sem_V(g_semId);
    }
}


int main()
{
    //klucz ipc
    key_t key = ftok(SCIEZKA_KLUCZA, KLUCZ_PROJ);
    if(key == -1) blad("init ftok");

    //tworzenie pamieci dzielonej
    int shmId = shmget(key, sizeof(StacjaInfo), IPC_CREAT | 0600);
    if(shmId == -1) blad("init shmget");

    //dolaczanie do pamieci
    StacjaInfo* info = (StacjaInfo*)shmat(shmId, nullptr, 0);
    if (info == (void*)-1) blad("init shmat");

    //inicjalizacja pamieci
    info->krzeslaWTrasie = 0;
    info->liczbaNarciarzyWKolejce = 0;
    info->liczbaNarciarzyWTrasie = 0;
    info->koniecSymulacji = false;
    for(int i=0; i<80; i++){
        info->stanKrzesla[i] = 0;
        info->ileOsobNaKrzesle[i] = 0;
    }

    //tworzenie semafora
    int semId = semget(key, 1, IPC_CREAT | 0600);
    if (semId == -1) blad("init semget");
    semun arg;
    arg.val = 1;
    if (semctl(semId, 0, SETVAL, arg) == -1) blad("init semctl SETVAL");

    //tworzenie kolejki komunikatow
    int msgId = msgget(key, IPC_CREAT | 0600);
    if (msgId == -1) blad("init msgget");

    //rejestracja handlera
    g_info = info;
    g_semId = semId;
    signal(SIGINT, sigintObsluga);

    cout << "[INIT] Zasoby IPC utworzone" << endl;

    //uruchamianie procesow
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

    //oczekiwanie na zakonczenie procesow
    for(int i = 0; i < 2 + 80 + LICZBA_NARCIARZY; i++){
        wait(nullptr);
    }

    //zwolnienie ipc
    cout << "[INIT] Usuwam zasoby IPC" << endl;
    shmctl(shmId, IPC_RMID, nullptr);
    semctl(semId, 0, IPC_RMID);
    msgctl(msgId, IPC_RMID, nullptr);

    //odlaczenie pamieci
    shmdt(info);
    cout << "[INIT] Koniec" << endl;
    return 0;
}
