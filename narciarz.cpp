#include "common.h"

int main(int argc, char* argv[])
{
    srand(time(nullptr) ^ getpid());

    //klucze ipc
    key_t keyStacja = ftok(SCIEZKA_KLUCZA_STACJA, KLUCZ_PROJ_STACJA);
    if (keyStacja == -1) blad("pracownik_dol ftok stacja");

    key_t keyWyciag = ftok(SCIEZKA_KLUCZA_WYCIAG, KLUCZ_PROJ_WYCIAG);
    if (keyWyciag == -1) blad("pracownik_dol ftok wyciag");

    key_t keyBramki = ftok(SCIEZKA_KLUCZA_BRAMKI, KLUCZ_PROJ_BRAMKI);
    if (keyBramki == -1) blad("pracownik_dol ftok bramki");

    //dolaczanie do pamieci dzielonej
    int shmIdStacja = shmget(keyStacja, sizeof(StacjaInfo), 0);
    if (shmIdStacja == -1) blad("narciarz shmget stacja");
    StacjaInfo* infoStacja = (StacjaInfo*)shmat(shmIdStacja, nullptr, 0);
    if (infoStacja == (void*)-1) blad("narciarz shmat stacja");

    int shmIdWyciag = shmget(keyWyciag, sizeof(WyciagInfo), 0);
    if (shmIdWyciag == -1) blad("narciarz shmget wyciag");
    WyciagInfo* infoWyciag = (WyciagInfo*)shmat(shmIdWyciag, nullptr, 0);
    if (infoWyciag == (void*)-1) blad("narciarz shmat wyciag");

    int shmIdBramki = shmget(keyBramki, sizeof(BramkiInfo), 0);
    if (shmIdBramki == -1) blad("narciarz shmget bramki");
    BramkiInfo* infoBramki = (BramkiInfo*)shmat(shmIdBramki, nullptr, 0);
    if (infoBramki == (void*)-1) blad("narciarz shmat bramki");

    //podlaczanie do semafora
    int semIdStacja = semget(keyStacja, 1, 0);
    if (semIdStacja == -1) blad("narciarz semget stacja");

    int semIdWyciag = semget(keyWyciag, 1, 0);
    if (semIdWyciag == -1) blad("narciarz semget wyciag");

    int semIdBramki = semget(keyBramki, 1, 0);
    if (semIdBramki == -1) blad("narciarz semget bramki");

    while (true){
        sem_P(semIdStacja);
        bool endSim = infoStacja->koniecSymulacji;
        sem_V(semIdStacja);

        if (endSim) {
            cout << "[Narciarz PID=" << getpid() << "] Koniec" << endl;;
            shmdt(infoStacja);
            shmdt(infoWyciag);
            shmdt(infoBramki);
            return 0;
        }

        int randomTime = rand() % 5 + 1;
        sleep(randomTime);
        break;
    }

    sem_P(semIdBramki);
    infoBramki->liczbaNarciarzyWKolejce++;
    int n = infoBramki->liczbaNarciarzyWKolejce;
    sem_V(semIdBramki);

    cout << "[Narciarz PID=" << getpid() << "] Ustawiam sie w kolejce. (razem=" << n << ")" << endl;

    //odlaczenie pamieci
    shmdt(infoStacja);
    shmdt(infoWyciag);
    shmdt(infoBramki);
    return 0;
}
