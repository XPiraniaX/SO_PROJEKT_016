#include "common.h"

static StacjaInfo* g_infoStacja = nullptr;
static int g_semIdStacja=-1;
static ZegarInfo* g_infoZegar = nullptr;
atomic<bool> koniecZegara(false);

//obsluga cygnalu ctrl+c
void sigintObsluga(int) {
    if (g_infoStacja) {
        sem_P(g_semIdStacja);
        g_infoStacja->koniecSymulacji = true;
        sem_V(g_semIdStacja);
    }
    koniecZegara = true;
}

//zegar
void Zegar() {

    cout << "\033[31m[Zegar] Otwieramy Stacje, godzina "<< GODZINA_OTWARCIA <<"\033[0m"<<endl;

    int czasZegara=0;
    while (g_infoZegar->godzina !=GODZINA_ZAMKNIECIA) {
        if(koniecZegara)
        {
            return;
        }
        this_thread::sleep_for(chrono::seconds(1));
        czasZegara++;
        g_infoZegar->minuta++;
        if (g_infoZegar->minuta == 60){
            g_infoZegar->godzina++;
            g_infoZegar->minuta=0;
            cout << "\033[31m[Zegar] Wybila godzina "<< g_infoZegar->godzina <<"\033[0m"<<endl;
        }

    }

    sem_P(g_semIdStacja);
    g_infoStacja->koniecSymulacji = true;
    sem_V(g_semIdStacja);

    cout << "\033[31m[Zegar] Zamykamy stacje\033[0m"<<endl;
}

//funckja do wypisywania logow
void wypiszLogi(){

    int fd = open(SCIEZKA_PLIKU_LOGI,O_RDONLY);
    if(fd == -1)blad("init wypisanie logi");

    constexpr size_t BUFSZ = 256;
    char buf[BUFSZ];
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buf, BUFSZ)) > 0) {
        cout.write(buf, bytesRead);
    }

    if (bytesRead == -1) blad("init read logi");

    close(fd);
}

int main()
{
    //                                  WALIDACJA USTAWIEN

    //ustawienia stacji
    if (LICZBA_DNI<1) blad("Zle ustawienia liczba dni");
    if (GODZINA_OTWARCIA>GODZINA_ZAMKNIECIA || GODZINA_OTWARCIA>24 || GODZINA_OTWARCIA<0 || GODZINA_ZAMKNIECIA>24 || GODZINA_ZAMKNIECIA<0 || GODZINA_OTWARCIA == GODZINA_ZAMKNIECIA)blad("Zle ustawienia godzin");
    //ustawienia turystow/narciarzy
    if (ILOSC_TURYSTOW_NA_OTWARCIU<0) blad("Zle ustawienia turystow na otwarciu");
    if (CZESTOTLIWOSC_TURYSTOW<0) blad("Zle ustawienia czestotliwosci turystow");
    if (SZANSA_NA_BYCIE_NARCIARZEM>100||SZANSA_NA_BYCIE_NARCIARZEM<0) blad("Zle ustawienia szansy na bycie narciarzem");
    if (SZANSA_NA_ZAKUP_BILETU_VIP>100||SZANSA_NA_ZAKUP_BILETU_VIP<0) blad("Zle ustawienia szansy na zakup vip");
    if (SZANSA_NA_POSIADANIE_DZIECKA>100||SZANSA_NA_POSIADANIE_DZIECKA<0) blad("Zle ustawienia szansy na posiadanie dziecka");
    //ustawienia kolejki
    if (MAX_DLUGOSC_KOLEJKI<=0) blad("Zle ustawienia max dlugosc kolejki");
    //ustawienia krzeselek i pracownikow
    if (SZANSA_NA_AWARIE_KOLEJKI>1000||SZANSA_NA_AWARIE_KOLEJKI<0) blad("Zle ustawienia szansy krzeselek i pracownikow");


    for (int i=0;i<LICZBA_DNI;i++)
    {
        //                                  INICJALIZACJA ZASOBOW

        //tworzenie plikow
        int fd = open(SCIEZKA_PLIKU_LOGI, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd==-1)blad("init stworzenie  logi");
        close(fd);

        int fks = open(SCIEZKA_KLUCZA_STACJA, O_WRONLY | O_CREAT | O_EXCL, 0666);
        if (fks==-1){
            if (errno == EEXIST){}
            else{
                blad("init stworzenie pliku key stacja");
            }
        }
        close(fks);

        int fkw = open(SCIEZKA_KLUCZA_WYCIAG, O_WRONLY | O_CREAT | O_EXCL, 0666);
        if (fkw==-1){
            if (errno == EEXIST){}
            else{
                blad("init stworzenie pliku key wyciag");
            }
        }
        close(fkw);

        int fkb = open(SCIEZKA_KLUCZA_BRAMKI, O_WRONLY | O_CREAT | O_EXCL, 0666);
        if (fkb==-1){
            if (errno == EEXIST){}
            else{
                blad("init stworzenie pliku key bramki");
            }
        }
        close(fkb);

        int fkk = open(SCIEZKA_KLUCZA_KASJER, O_WRONLY | O_CREAT | O_EXCL, 0666);
        if (fkk==-1){
            if (errno == EEXIST){}
            else{
                blad("init stworzenie pliku key kasjer");
            }
        }
        close(fkk);

        int fkz = open(SCIEZKA_KLUCZA_ZEGAR, O_WRONLY | O_CREAT | O_EXCL, 0666);
        if (fkz==-1){
            if (errno == EEXIST){}
            else{
                blad("init stworzenie pliku key zegar");
            }
        }
        close(fkz);

        //klucze ipc
        key_t keyStacja = ftok(SCIEZKA_KLUCZA_STACJA, KLUCZ_PROJ_STACJA);
        if(keyStacja == -1) blad("init ftok stacji");

        key_t keyWyciag = ftok(SCIEZKA_KLUCZA_WYCIAG, KLUCZ_PROJ_WYCIAG);
        if(keyWyciag == -1) blad("init ftok wyciagu");

        key_t keyBramki = ftok(SCIEZKA_KLUCZA_BRAMKI, KLUCZ_PROJ_BRAMKI);
        if(keyBramki == -1) blad("init ftok bramek");

        key_t keyKasjer = ftok(SCIEZKA_KLUCZA_KASJER, KLUCZ_PROJ_KASJER);
        if (keyKasjer == -1) blad("init ftok kasjer");

        key_t keyZegar = ftok(SCIEZKA_KLUCZA_ZEGAR, KLUCZ_PROJ_KASJER);
        if (keyZegar == -1) blad("init ftok zegar");


        //tworzenie pamieci dzielonej
        int shmIdStacja = shmget(keyStacja, sizeof(StacjaInfo), IPC_CREAT | 0600);
        if(shmIdStacja == -1) blad("init shmget stacji");

        int shmIdWyciag = shmget(keyWyciag, sizeof(WyciagInfo), IPC_CREAT | 0600);
        if(shmIdWyciag == -1) blad("init shmget wyciagu");

        int shmIdBramki = shmget(keyBramki, sizeof(BramkiInfo), IPC_CREAT | 0600);
        if(shmIdBramki == -1) blad("init shmget bramek");

        int shmIdZegar = shmget(keyZegar, sizeof(ZegarInfo), IPC_CREAT | 0600);
        if(shmIdZegar == -1) blad("init shmget zegar");

        //dolaczanie do pamieci
        StacjaInfo* infoStacja = (StacjaInfo*)shmat(shmIdStacja, nullptr, 0);
        if (infoStacja == (void*)-1) blad("init shmat stacji");

        WyciagInfo* infoWyciag = (WyciagInfo*)shmat(shmIdWyciag, nullptr, 0);
        if (infoWyciag == (void*)-1) blad("init shmat wyciagu");

        BramkiInfo* infoBramki = (BramkiInfo*)shmat(shmIdBramki, nullptr, 0);
        if (infoBramki == (void*)-1) blad("init shmat bramek");

        ZegarInfo* infoZegar = (ZegarInfo*)shmat(shmIdZegar, nullptr, 0);
        if (infoZegar == (void*)-1) blad("init shmat zegar");

        //inicjalizacja pamieci
        infoStacja->koniecSymulacji = false;

        infoWyciag->krzeslaWTrasie = 0;
        infoWyciag->liczbaNarciarzyWTrasie = 0;
        for(int i=0; i<80; i++){
            infoWyciag->stanKrzesla[i] = 0;
            infoWyciag->ileOsobNaKrzesle[i] = 0;
            infoWyciag->pidKrzesel[i] = -1;
        }
        infoBramki->liczbaNarciarzyWKolejce = 0;
        infoBramki->przod=0;
        infoBramki->tyl=0;
        infoZegar->godzina = GODZINA_OTWARCIA;
        infoZegar->minuta = 0;

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

        int semIdBramkiWejscie = semget(keyBramki, 1, IPC_CREAT | 0600);
        if (semIdBramkiWejscie == -1) blad("init bramek");
        semun argBramkiWejscie;
        argBramkiWejscie.val = 4;
        if (semctl(semIdBramkiWejscie, 0, SETVAL, argBramkiWejscie) == -1) blad("init semctl bramek wejscie");

        int semIdKasjer = semget(keyKasjer, 1, IPC_CREAT | 0600);
        if (semIdKasjer == -1) blad("init semget kasjera");
        semun argKasjer;
        argKasjer.val = 1;
        if (semctl(semIdKasjer, 0, SETVAL, argKasjer) == -1) blad("init semctl SETVAL kasjera");

        //tworzenie i dolaczanie do kolejki komunikatow
        int msgIdWyciag = msgget(keyWyciag, IPC_CREAT | 0600);
        if (msgIdWyciag == -1) blad("init msgget wyciag");

        int msgIdNarciarz = msgget(keyWyciag, IPC_CREAT | 0600);
        if (msgIdNarciarz == -1) blad("init msgget narciarz");

        int msgIdKasjer = msgget(keyKasjer, IPC_CREAT | 0600);
        if (msgIdKasjer == -1) blad("init msgget kasjer");

        cout << "[INIT] Zasoby IPC utworzone" << endl;

        //rejestracja handlera
        g_infoStacja = infoStacja;
        g_semIdStacja = semIdStacja;
        signal(SIGINT, sigintObsluga);

        srand(time(NULL));

        //                                  START SYMULACJI

        //uruchomienie zegara
        g_infoZegar=infoZegar;
        thread timerThread(Zegar);

        //uruchamianie procesow
        int sumaProcesow = 0;

        //pracownicy
        pid_t pd = fork();
        if (pd == 0) {
            execlp("./pracownik_dol", "pracownik_dol", nullptr);
            blad("execlp pracownik_dol");
        }
        sumaProcesow++;

        pid_t pg = fork();
        if (pg == 0) {
            execlp("./pracownik_gora", "pracownik_gora", nullptr);
            blad("execlp pracownik_gora");
        }
        sumaProcesow++;

        //kasjer
        pid_t pk = fork();
        if (pk == 0) {
            execlp("./kasjer", "kasjer", nullptr);
            blad("execlp kasjer");
        }
        sumaProcesow++;

        //krzeselka
        vector<pid_t> pidKrzesel;
        for(int i=0; i<80; i++){
            pid_t pk = fork();
            if(pk == 0){
                char buf[16];
                sprintf(buf, "%d", i);
                execlp("./krzeslo", "krzeslo", buf, nullptr);
                blad("execlp krzeslo");
            }else{
                pidKrzesel.push_back(pk);
                sem_P(semIdWyciag);
                infoWyciag->pidKrzesel[i]=pk;
                sem_V(semIdWyciag);
            }
        }
        cout << "\033[32m[Krzesla] START\033[0m" << endl;
        sumaProcesow += 80;

        //turysci na start
        vector<pid_t> pidTurystow;
        int liczbaTurystow;
        for( liczbaTurystow=0; liczbaTurystow<ILOSC_TURYSTOW_NA_OTWARCIU; liczbaTurystow++){
            pid_t pt = fork();
            if(pt == 0){
                char buf[16];
                sprintf(buf, "%d", liczbaTurystow);
                execlp("./turysta", "turysta",buf, nullptr);
                blad("execlp turysta");
            }else if (pt>0){
                pidTurystow.push_back(pt);
            }

        }
        sumaProcesow+=ILOSC_TURYSTOW_NA_OTWARCIU;

        //generator tursytow
        while(true) {
            sem_P(semIdStacja);
            if (infoStacja->koniecSymulacji == true) {
                sem_V(semIdStacja);
                break;
            }
            sem_V(semIdStacja);

            int odst = rand() % (CZESTOTLIWOSC_TURYSTOW+1);
            int czasSnu=0;
            while (odst>=czasSnu){
                sem_P(semIdStacja);
                if (infoStacja->koniecSymulacji == true) {
                    sem_V(semIdStacja);
                    break;
                }
                sem_V(semIdStacja);
                sleep(1);
                czasSnu++;
            }

            pid_t pt = fork();
            if (pt == 0) {
                char buf[16];
                sprintf(buf, "%d", liczbaTurystow);
                execlp("./turysta", "turysta", buf, nullptr);
                blad("execlp turysta");
            } else if (pt > 0) {
                pidTurystow.push_back(pt);
                sumaProcesow++;
                liczbaTurystow++;
            }
        }

        //                                  ZAMYKANIE STACJII

        //oczekiwanie na zakonczenie zegara
        timerThread.join();

        //wyslanie komunikatu o zakonczeniu do kasjera
        msgKasjer koniecKasjer;
        koniecKasjer.mtype=99;
        if (msgsnd(msgIdKasjer, &koniecKasjer, sizeof(koniecKasjer) - sizeof(long), 0) == -1) {
            blad("[INIT] msgsnd kasjer error");
        }

        //wyslanie komunikatu o zakonczeniu do pracownika_dol
        msgWyciag koniecPracownicy;
        koniecPracownicy.mtype=99999;
        if (msgsnd(msgIdWyciag, &koniecPracownicy, sizeof(koniecPracownicy) - sizeof(long), 0) == -1) {
            blad("[INIT] msgsnd pracownicy error");
        }

        //oczekiwanie na zakonczenie pracownika gora
        waitpid(pg,nullptr,0);

        //zakonczenie procesow krzesel
        for (pid_t pid : pidKrzesel) {
            kill(pid, SIGTERM);
        }
        cout << "\033[32m[Krzesla] KONIEC\033[0m" << endl;

        //wyproszenie pozostalych turystow
        for (pid_t pid : pidTurystow) {
            kill(pid, SIGTERM);
        }

        //odstep aby kazdy narciarz wyslal komunikat o zjezdzie przed komunikatem o usunieciu zasobów( mozna go usunac tylko poprawia wyglad wyjscia po zakonczeniu programu)
        sleep(20);

        cout << "\033[37m[Bramki] Zapisy karnetow: \033[0m" << endl;
        wypiszLogi();

        //zwolnienie ipc
        cout << "[INIT] Usuwam zasoby IPC" << endl;
        shmctl(shmIdStacja, IPC_RMID, nullptr);
        shmctl(shmIdWyciag, IPC_RMID, nullptr);
        shmctl(shmIdBramki, IPC_RMID, nullptr);
        shmctl(shmIdZegar, IPC_RMID, nullptr);

        semctl(semIdStacja, 0, IPC_RMID);
        semctl(semIdWyciag, 0, IPC_RMID);
        semctl(semIdBramki, 0, IPC_RMID);
        semctl(semIdBramkiWejscie, 0, IPC_RMID);
        semctl(semIdKasjer, 0, IPC_RMID);

        msgctl(msgIdWyciag, IPC_RMID, nullptr);
        msgctl(msgIdNarciarz, IPC_RMID, nullptr);
        msgctl(msgIdKasjer, IPC_RMID, nullptr);

        //odlaczenie pamieci
        shmdt(infoStacja);
        shmdt(infoWyciag);
        shmdt(infoBramki);
        shmdt(infoZegar);
        cout << "[INIT] Koniec" << endl;
    }
    return 0;
}
