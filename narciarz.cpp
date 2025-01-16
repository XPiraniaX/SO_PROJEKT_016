#include "common.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        blad("[narciarz] Brak argumentu: liczba zjazdow!\n");
        return 1;
    }

    int zjazdy = atoi(argv[1]);

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


    cout << "[Narciarz] START z " << zjazdy << " zjazdami" << endl;
    while(zjazdy > 0) {

        sem_P(semIdBramki);
        if (infoBramki->liczbaNarciarzyWKolejce < 100) {
            infoBramki->liczbaNarciarzyWKolejce++;
            sem_V(semIdBramki);

            cout << "[Narciarz] Wchodze przez bramki. Zostalo zjazdow=" << zjazdy << endl;
            zjazdy--;
        } else {
            sem_V(semIdBramki);
            cout << "[Narciarz] Za duza kolejka (>=100), czekam..." << endl;
            sleep(3);
            continue;
        }

        // 2. Ustawiamy się w kolejce do wyciagu (Twoja dotychczasowa logika - np.
        // sem / kolejka ?). Dla uproszczenia sleep(1).
        sleep(1);

        // 3. czekamy na krzeslo -> 40s jazdy w gore
        std::cout << "[Narciarz] Jade w gore (40s)...\n";
        sleep(40);
        std::cout << "[Narciarz] Jestem na gorze.\n";

        // 4. zjezdzam w dol 20s
        std::cout << "[Narciarz] Zjezdzam w dol (20s)...\n";
        sleep(20);

        // 5. Wychodze z bramek (jak wracam na dol, moge zmniejszyc infoBramki->liczbaNarciarzyWKolejce)
        sem_P(semIdBramki);
        if (infoBramki->liczbaNarciarzyWKolejce > 0)
            infoBramki->liczbaNarciarzyWKolejce--;
        sem_V(semIdBramki);
    }

    // Gdy zjazdy=0, koniec
    std::cout << "[Narciarz] Skończyłem wszystkie zjazdy -> KONIEC.\n";

    //odlaczenie pamieci
    shmdt(infoStacja);
    shmdt(infoWyciag);
    shmdt(infoBramki);
    return 0;
}
