#include "common.h"

int main(int argc, char* argv[])
{
    //                                  WALIDACJA ARGUMENTOW

    if (argc < 2) {
        blad("[Turysta] Podaj id krzesla w argumencie!");
    }
    int tId = atoi(argv[1]);

    srand(time(NULL)^getpid());

    //                                  INICJALIZACJA ZASOBOW

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

    //                                  START SYMULACJI

    //sprawdzanie flagi konca symulacji
    if (infoStacja->koniecSymulacji)
    {
        cout << "[Turysta #"<< (tId+1) <<"] Stacja się zamyka, ide do domu KONIEC" << endl;
        shmdt(infoStacja);
        return 0;
    }

    //zwiedzanie stacji
    sleep(rand()%25);

    //decyzja czy zostaje narciarzem
    if ((rand()%100+1)>SZANSA_NA_BYCIE_NARCIARZEM)
    {
        cout << "[Turysta #"<< (tId+1) <<"] Wychodzę, KONIEC" << endl;
        shmdt(infoStacja);
        return 0;
    }

    sem_P(semIdKasjer);

    //wysylamy prosbe do kasjera
    msgKasjer req;
    req.mtype = 1;
    req.turystaId = 100+tId;
    int losowyIndeks = rand() % 4;
    req.liczbaZjazdow = wyborBiletu[losowyIndeks];
    req.wiek=(rand()%68+8); // wiek turystow (od 8 do 75, ponizej 8 za darmo)
    if ((rand()%100)+1<=SZANSA_NA_ZAKUP_BILETU_VIP){
        req.liczbaZjazdow = 1000;
    }
    while (true)
    {
        //sprawdzanie flagi konca symulacji
        if (infoStacja->koniecSymulacji)
        {
            cout << "[Turysta #"<< (tId+1) <<"] Stacja się zamyka, ide do domu KONIEC" << endl;
            shmdt(infoStacja);
            return 0;
        }
        //otrzmanie wyslanie komunikatu do kasjera
        if (msgsnd(msgIdKasjer, &req, sizeof(msgKasjer) - sizeof(long), IPC_NOWAIT) == -1) {
            if (errno == EAGAIN)
            {
                sleep(1);
                continue;
            }
            blad("[Turysta] msgsnd kasjer error");
        }
        break;
    }

    //sprawdzanie flagi konca symulacji
    if (infoStacja->koniecSymulacji)
    {
        cout << "[Turysta #"<< (tId+1) <<"] Stacja się zamyka, ide do domu KONIEC" << endl;
        sem_V(semIdKasjer);
        shmdt(infoStacja);
        return 0;
    }

    cout << "\033[34m[Turysta #"<< (tId+1) <<"] Wyslano prosbe o karnet do kasjera\033[0m" << endl;

    //czekamy na odpowiedz
    msgKasjer resp;
    if (msgrcv(msgIdKasjer, &resp, sizeof(msgKasjer) - sizeof(long), 100+tId, 0)==-1){
        blad("[Turysta msgrcv kasjer error]");
    }

    if (resp.liczbaZjazdow == 100){
        cout << "\033[34m[Turysta #"<< (tId+1) <<"] Otrzymano karnet calodniowy\033[0m" << endl;
    }
    else if (resp.liczbaZjazdow == 1000){
        cout << "\033[34m[Turysta #"<< (tId+1) <<"] Otrzymano karnet VIP\033[33m [V.I.P]\033[0m" << endl;
    }
    else{
        cout << "\033[34m[Turysta #"<< (tId+1) <<"] Otrzymano karnet na " << resp.liczbaZjazdow << " zjazdow\033[0m" << endl;
    }

    sem_V(semIdKasjer);

    //symulacja przebrania sie i zostania narciarzem
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
        if (resp.liczbaZjazdow == 100){
            cout << "[Turysta #"<< (tId+1) <<"] Zostaje narciarzem z biletem calodniowym" << endl;
        }
        else{
            cout << "[Turysta #"<< (tId+1) <<"] Zostaje narciarzem z " << resp.liczbaZjazdow << " zjazdami" << endl;
        }

    }
    //                                  KONIEC TURYSTY
    shmdt(infoStacja);
    return 0;
}
