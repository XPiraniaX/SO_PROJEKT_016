#include "common.h"

int main(int argc, char* argv[])
{
    if (argc < 3) {
        blad("[narciarz] Brak argumentow: liczba zjazdow/id narciarza!");
        return 1;
    }

    int zjazdy = atoi(argv[1]);
    int nId = atoi(argv[2]);
    //klucze ipc
    key_t keyStacja = ftok(SCIEZKA_KLUCZA_STACJA, KLUCZ_PROJ_STACJA);
    if (keyStacja == -1) blad("narciarz ftok stacja");

    key_t keyWyciag = ftok(SCIEZKA_KLUCZA_WYCIAG, KLUCZ_PROJ_WYCIAG);
    if (keyWyciag == -1) blad("narciarz ftok wyciag");

    key_t keyBramki = ftok(SCIEZKA_KLUCZA_BRAMKI, KLUCZ_PROJ_BRAMKI);
    if (keyBramki == -1) blad("narciarz ftok bramki");

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

    int semIdBramki = semget(keyBramki, 1, 0);
    if (semIdBramki == -1) blad("narciarz semget bramki");

    int semIdBramkiWejscie = semget(keyBramki, 1, 0);
    if (semIdBramkiWejscie == -1) blad("narciarz semget bramki");

    //dolaczanie do kolejki komunikatow
    int msgIdNarciarz = msgget(keyWyciag, 0);
    if (msgIdNarciarz == -1) blad("narciarz msgget narciarz");

    cout << "[Narciarz #" << (nId+1) << "] START z " << zjazdy << " zjazdami" << endl;

    while(zjazdy > 0) {

        sem_P(semIdStacja);
        bool endSim = infoStacja->koniecSymulacji;
        sem_V(semIdStacja);

        if (endSim){
            cout << "[Narciarz #" << (nId+1) << "] KONIEC" << endl;
            shmdt(infoStacja);
            shmdt(infoWyciag);
            shmdt(infoBramki);
            return 0;
        }

        sem_P(semIdBramkiWejscie);
        sem_P(semIdBramki);
        if (czyKolejkaPelna(infoBramki)){
            sem_V(semIdBramki);
            cout << "[Narciarz #" << (nId+1) << "] Kolejka pelna, próbuje znowu za 0.1s" << endl;
            sleep(0.1);
            continue;
        }

        pushNarciarz(infoBramki,nId);
        zjazdy--;
        cout << "[Narciarz #" << (nId+1) << "] Przeszedlem przez bramke do kolejki" << endl;
        sem_V(semIdBramki);
        sem_V(semIdBramkiWejscie);

        //oczekiwanie na wysiadaj
        msgNarciarz msg1;
        if (msgrcv(msgIdNarciarz, &msg1, sizeof(msgNarciarz)-sizeof(long), 1000+nId,0)==-1){
            blad("[Narciarz] msgrcv wysiadaj error");
            break;
        }
        cout << "[Narciarz #" << (nId+1) << "] Wysiadam na gorze, i zjezdzam" << endl;

        sem_P(semIdStacja);
        endSim = infoStacja->koniecSymulacji;
        sem_V(semIdStacja);

        if (endSim){
            //symulacja ostatniego zjazdu
            sleep(5);

            cout << "[Narciarz #" << (nId+1) << "] Zjechałem, idę do domu" << endl;
            cout << "[Narciarz #" << (nId+1) << "] KONIEC" << endl;
            shmdt(infoStacja);
            shmdt(infoWyciag);
            shmdt(infoBramki);
            return 0;
        }
        else{
            //symulacja zjazdu
            sleep(5);

            cout << "[Narciarz #" << (nId+1) << "] Zjechałem, wracam na stacje" << endl;

            sleep(5);
        }
    }

    cout << "[Narciarz #"<< (nId+1) << "] Skończyłem wszystkie zjazdy KONIEC" << endl;

    //odlaczenie pamieci
    shmdt(infoStacja);
    shmdt(infoWyciag);
    shmdt(infoBramki);
    return 0;
}
