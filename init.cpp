#include "common.h"

static StacjaInfo* g_infoStacja = nullptr;
static int g_semIdStacja=-1;
static WyciagInfo* g_infoWyciag = nullptr;
static int g_semIdWyciag=-1;
static vector<pid_t> g_pidKrzesel;
static pid_t g_pidPracownikGora;
static bool koniecZegara = false;

void sigintObsluga(int) {
    if (g_infoStacja) {
        sem_P(g_semIdStacja);
        g_infoStacja->koniecSymulacji = true;
        sem_V(g_semIdStacja);
    }
    koniecZegara = true;
}

void Zegar() {
    int czasZegara=0;
    while (czasZegara < DLUGOSC_SYMULACJI){
        if(koniecZegara) return;
        this_thread::sleep_for(chrono::seconds(1));
        czasZegara++;
    }

    sem_P(g_semIdStacja, 0);
    g_infoStacja->koniecSymulacji = true;
    sem_V(g_semIdStacja);

    cout << "[Zegar] Upłynęło " << DLUGOSC_SYMULACJI << "s, zamykamy stacje"<<endl;

    waitpid(g_pidPracownikGora,nullptr,0);

    int i=0;
    for (pid_t pid : g_pidKrzesel) {
        sem_P(g_semIdWyciag);
        if (g_infoWyciag->stanKrzesla[i] != 1 ){
            kill(pid, SIGUSR1);
        }
        sem_V(g_semIdWyciag);
        i++;
    }
}

int main()
{
    //klucze ipc
    key_t keyStacja = ftok(SCIEZKA_KLUCZA_STACJA, KLUCZ_PROJ_STACJA);
    if(keyStacja == -1) blad("init ftok stacji");

    key_t keyWyciag = ftok(SCIEZKA_KLUCZA_WYCIAG, KLUCZ_PROJ_WYCIAG);
    if(keyWyciag == -1) blad("init ftok wyciagu");

    key_t keyBramki = ftok(SCIEZKA_KLUCZA_BRAMKI, KLUCZ_PROJ_BRAMKI);
    if(keyBramki == -1) blad("init ftok bramek");

    //tworzenie pamieci dzielonej
    int shmIdStacja = shmget(keyStacja, sizeof(StacjaInfo), IPC_CREAT | 0600);
    if(shmIdStacja == -1) blad("init shmget stacji");

    int shmIdWyciag = shmget(keyWyciag, sizeof(WyciagInfo), IPC_CREAT | 0600);
    if(shmIdWyciag == -1) blad("init shmget wyciagu");

    int shmIdBramki = shmget(keyBramki, sizeof(BramkiInfo), IPC_CREAT | 0600);
    if(shmIdBramki == -1) blad("init shmget bramek");

    //dolaczanie do pamieci
    StacjaInfo* infoStacja = (StacjaInfo*)shmat(shmIdStacja, nullptr, 0);
    if (infoStacja == (void*)-1) blad("init shmat stacji");

    WyciagInfo* infoWyciag = (WyciagInfo*)shmat(shmIdWyciag, nullptr, 0);
    if (infoWyciag == (void*)-1) blad("init shmat wyciagu");

    BramkiInfo* infoBramki = (BramkiInfo*)shmat(shmIdBramki, nullptr, 0);
    if (infoBramki == (void*)-1) blad("init shmat bramek");

    //inicjalizacja pamieci
    infoStacja->koniecSymulacji = false;

    infoWyciag->krzeslaWTrasie = 0;
    infoWyciag->liczbaNarciarzyWTrasie = 0;
    for(int i=0; i<80; i++){
        infoWyciag->stanKrzesla[i] = 0;
        infoWyciag->ileOsobNaKrzesle[i] = 0;
    }
    infoBramki->liczbaNarciarzyWKolejce = 0;

    //tworzenie i podlaczenie semaforów
    int semIdStacja = semget(keyStacja, 1, IPC_CREAT | 0600);
    if (semIdStacja == -1) blad("init semget stacji");
    semun argStacja;
    argStacja.val = 1;
    if (semctl(semIdStacja, 0, SETVAL, argStacja) == -1) blad("init semctl SETVAL stacji");

    int semIdWyciag = semget(keyWyciag, 1, IPC_CREAT | 0600);
    if (semIdWyciag == -1) blad("init semget wyciagu");
    semun argWyciag;
    argWyciag.val = 1;
    if (semctl(semIdWyciag, 0, SETVAL, argWyciag) == -1) blad("init semctl SETVAL wycaigu");

    int semIdBramki = semget(keyBramki, 1, IPC_CREAT | 0600);
    if (semIdBramki == -1) blad("init bramek");
    semun argBramki;
    argBramki.val = 1;
    if (semctl(semIdBramki, 0, SETVAL, argBramki) == -1) blad("init semctl bramek");

    //tworzenie i dolaczanie do kolejki komunikatow
    int msgIdWyciag = msgget(keyWyciag, IPC_CREAT | 0600);
    if (msgIdWyciag == -1) blad("init msgget wyciag");

    //rejestracja handlera
    g_infoStacja = infoStacja;
    g_semIdStacja = semIdStacja;
    g_infoWyciag = infoWyciag;
    g_semIdWyciag = semIdWyciag;
    signal(SIGINT, sigintObsluga);

    thread timerThread(Zegar);

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
    }else{
        g_pidPracownikGora=pg;
    }

    for(int i=0; i<80; i++){
        pid_t pk = fork();
        if(pk == 0){
            char buf[16];
            sprintf(buf, "%d", i);
            execlp("./krzeslo", "krzeslo", buf, nullptr);
            blad("execlp krzeslo");
        }else{
            g_pidKrzesel.push_back(pk);
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
    shmctl(shmIdStacja, IPC_RMID, nullptr);
    shmctl(shmIdWyciag, IPC_RMID, nullptr);
    shmctl(shmIdBramki, IPC_RMID, nullptr);

    semctl(semIdStacja, 0, IPC_RMID);
    semctl(semIdWyciag, 0, IPC_RMID);
    semctl(semIdBramki, 0, IPC_RMID);

    msgctl(msgIdWyciag, IPC_RMID, nullptr);


    //odlaczenie pamieci
    shmdt(infoStacja);
    shmdt(infoWyciag);
    shmdt(infoBramki);

    if (timerThread.joinable()) {
        timerThread.join();
    }

    cout << "[INIT] Koniec" << endl;
    return 0;
}
