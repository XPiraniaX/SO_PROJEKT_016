#include "common.h"

void sigusrObsluga(int){}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        blad("[krzeslo] Podaj id krzesla w argumencie!");
        return 1;
    }
    int kIdx = atoi(argv[1]);

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

    //cout << "[Krzeslo #" << (kIdx+1) << "] START" << endl;

    while(true){

        sem_P(semIdStacja);
        sem_P(semIdBramki);
        bool endSim = infoStacja->koniecSymulacji && infoBramki->liczbaNarciarzyWKolejce==0;
        sem_V(semIdStacja);
        sem_V(semIdBramki);

        if (endSim){
            cout << "[Krzeslo #" << (kIdx + 1) << "] Koniec" << endl;
            shmdt(infoStacja);
            shmdt(infoWyciag);
            shmdt(infoBramki);
            return 0;
        }

        //oczekiwanie na start jazdy
        msgWyciag msg;
        if (msgrcv(msgIdWyciag, &msg, sizeof(msgWyciag)-sizeof(long), 100 + kIdx, 0) == -1) {
            if (errno == EINTR) {
                sem_P(semIdStacja);
                endSim = infoStacja->koniecSymulacji;
                sem_V(semIdStacja);

                if (endSim) {
                    cout << "[Krzeslo #" << (kIdx + 1) << "] Koniec (sygnal)" <<endl;
                    shmdt(infoStacja);
                    shmdt(infoWyciag);
                    shmdt(infoBramki);
                    return 0;
                }
                continue;
            } else {
                blad("[krzeslo] msgrcv start error");
                sleep(1);
                continue;
            }
        }

        int ileOsob = msg.liczbaOsob;
        cout << "[Krzeslo #" << (kIdx+1) << "] Ruszam w gore z " << ileOsob << " osobami" << endl;


        sleep(40);

        //komunikat na górze
        msgWyciag msg2;
        msg2.mtype = 200 + kIdx;
        msg2.nrKrzesla = kIdx;
        msg2.liczbaOsob= ileOsob;
        if (msgsnd(msgIdWyciag, &msg2, sizeof(msgWyciag)-sizeof(long), 0)==-1){
            blad("[Krzeslo] msgsnd pracownik gorna stacja error");
            return 1;
        }

        //oczekiwanie na wracaj
        msgWyciag msg3;
        if(msgrcv(msgIdWyciag, &msg3, sizeof(msgWyciag)-sizeof(long), 300+kIdx, 0) == -1){
            blad("[krzeslo] msgrcv gorna stacja  return");
            sleep(1);
            continue;
        }
        cout << "[Krzeslo #" << (kIdx+1) << "] Wracam na dol" << endl;

        sem_P(semIdWyciag);
        infoWyciag->stanKrzesla[kIdx] = 2;
        infoWyciag->ileOsobNaKrzesle[kIdx] = 0;
        sem_V(semIdWyciag);

        sleep(40);

        sem_P(semIdWyciag);
        infoWyciag->stanKrzesla[kIdx] = 0;
        sem_V(semIdWyciag);

        cout << "[Krzeslo #" << (kIdx+1) << "] Jestem wolne na dole" << endl;
    }
}
