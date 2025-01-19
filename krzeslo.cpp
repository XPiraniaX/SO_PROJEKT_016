#include "common.h"

static volatile sig_atomic_t sigusrOdebrany = 0;

void sigusrObsluga(int)
{
    sigusrOdebrany = 1;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        blad("[krzeslo] Podaj id krzesla w argumencie!");
        return 1;
    }
    int kId = atoi(argv[1]);

    signal(SIGUSR1,sigusrObsluga);

    //klucze ipc
    key_t keyStacja = ftok(SCIEZKA_KLUCZA_STACJA, KLUCZ_PROJ_STACJA);
    if (keyStacja == -1) blad("pracownik_dol ftok stacja");

    key_t keyWyciag = ftok(SCIEZKA_KLUCZA_WYCIAG, KLUCZ_PROJ_WYCIAG);
    if (keyWyciag == -1) blad("pracownik_dol ftok wyciag");

    key_t keyBramki = ftok(SCIEZKA_KLUCZA_BRAMKI, KLUCZ_PROJ_BRAMKI);
    if (keyBramki == -1) blad("pracownik_dol ftok bramki");

    //dolaczanie do pamieci dzielonej
    int shmIdStacja = shmget(keyStacja, sizeof(StacjaInfo), 0);
    if (shmIdStacja == -1) blad("krzeslo shmget stacja");
    StacjaInfo* infoStacja = (StacjaInfo*)shmat(shmIdStacja, nullptr, 0);
    if (infoStacja == (void*)-1) blad("krzeslo shmat stacja");

    int shmIdWyciag = shmget(keyWyciag, sizeof(WyciagInfo), 0);
    if (shmIdWyciag == -1) blad("krzeslo shmget wyciag");
    WyciagInfo* infoWyciag = (WyciagInfo*)shmat(shmIdWyciag, nullptr, 0);
    if (infoWyciag == (void*)-1) blad("krzeslo shmat wyciag");

    int shmIdBramki = shmget(keyBramki, sizeof(BramkiInfo), 0);
    if (shmIdBramki == -1) blad("krzeslo shmget bramki");
    BramkiInfo* infoBramki = (BramkiInfo*)shmat(shmIdBramki, nullptr, 0);
    if (infoBramki == (void*)-1) blad("krzeslo shmat bramki");

    //podlaczanie do semafora
    int semIdStacja = semget(keyStacja, 1, 0);
    if (semIdStacja == -1) blad("krzeslo semget stacja");

    int semIdWyciag = semget(keyWyciag, 1, 0);
    if (semIdWyciag == -1) blad("krzeslo semget wyciag");

    int semIdBramki = semget(keyBramki, 1, 0);
    if (semIdBramki == -1) blad("krzeslo semget bramki");

    //dolaczanie do kolejki komunikatow
    int msgIdWyciag = msgget(keyWyciag, 0);
    if (msgIdWyciag == -1) blad("krzeslo msgget wyciag");

    //cout << "\033[32m[Krzeslo #" << (kId+1) << "] START\033[0m" << endl;

    while(true){

        if (sigusrOdebrany) {
            break;
        }

        //oczekiwanie na start jazdy
        msgWyciag msg1;
        if (msgrcv(msgIdWyciag, &msg1, sizeof(msgWyciag)-sizeof(long),
                   100 + kId, 0) == -1)
        {
            if (errno == EINTR && sigusrOdebrany) {
                break;
            }
            blad("[Krzeslo] msgrcv start error");
            continue;
        }

        int ileOsob = msg1.liczbaOsob;
        cout << "\033[32m[Krzeslo #" << (kId+1) << "] Ruszam w gore z " << ileOsob << " osobami\033[0m" << endl;


        for (int i=0; i<40; i++) {
            if (sigusrOdebrany) {
                cout << "\033[32m[Krzeslo #" << (kId + 1) << "] KONIEC\033[0m" << endl;
                shmdt(infoStacja);
                shmdt(infoWyciag);
                shmdt(infoBramki);
                return 0;
            }
            sleep(1);
        }

        //komunikat na górze
        msgWyciag msg2;
        msg2.mtype = 200 + kId;
        msg2.nrKrzesla = kId;
        msg2.liczbaOsob= ileOsob;
        copy(begin(msg1.idNarciarzyNaKrzesle), end(msg1.idNarciarzyNaKrzesle), begin(msg2.idNarciarzyNaKrzesle));
        if (msgsnd(msgIdWyciag, &msg2, sizeof(msgWyciag)-sizeof(long), 0)==-1){
            blad("[Krzeslo] msgsnd pracownik gorna stacja error");
            return 1;
        }

        //oczekiwanie na wracaj
        msgWyciag msg3;
        if (msgrcv(msgIdWyciag, &msg3, sizeof(msgWyciag)-sizeof(long),
                   300 + kId, 0) == -1)
        {
            if (errno == EINTR && sigusrOdebrany) {
                break;
            }
            blad("[Krzeslo] msgrcv gorna stacja wracaj error");
            continue;
        }
        cout << "\033[32m[Krzeslo #" << (kId+1) << "] Wracam na dol\033[0m" << endl;

        sem_P(semIdWyciag);
        infoWyciag->stanKrzesla[kId] = 2;
        infoWyciag->ileOsobNaKrzesle[kId] = 0;
        sem_V(semIdWyciag);

        for (int i=0; i<40; i++) {
            if (sigusrOdebrany) {
                cout << "\033[32m[Krzeslo #" << (kId + 1) << "] KONIEC\033[0m" << endl;
                shmdt(infoStacja);
                shmdt(infoWyciag);
                shmdt(infoBramki);
                return 0;
            }
            sleep(1);
        }

        sem_P(semIdWyciag);
        infoWyciag->stanKrzesla[kId] = 0;
        sem_V(semIdWyciag);

        cout << "\033[0m[Krzeslo #" << (kId+1) << "] Jestem wolne na dole\033[0m" << endl;
    }
    cout << "\033[32m[Krzeslo #" << (kId + 1) << "] KONIEC\033[0m" << endl;
    shmdt(infoStacja);
    shmdt(infoWyciag);
    shmdt(infoBramki);
    return 0;
}