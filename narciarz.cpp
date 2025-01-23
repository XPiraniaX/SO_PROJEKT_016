#include "common.h"

int main(int argc, char* argv[])
{
    if (argc < 3) {
        blad("[narciarz] Brak argumentow: liczba zjazdow/id narciarza!");
        return 1;
    }

    srand(time(NULL)^getpid());

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

    int shmIdBramki = shmget(keyBramki, sizeof(BramkiInfo), 0);
    if (shmIdBramki == -1) blad("narciarz shmget bramki");
    BramkiInfo* infoBramki = (BramkiInfo*)shmat(shmIdBramki, nullptr, 0);
    if (infoBramki == (void*)-1) blad("narciarz shmat bramki");

    //podlaczanie do semafora

    int semIdBramki = semget(keyBramki, 1, 0);
    if (semIdBramki == -1) blad("narciarz semget bramki");

    int semIdBramkiWejscie = semget(keyBramki, 1, 0);
    if (semIdBramkiWejscie == -1) blad("narciarz semget bramki");

    //dolaczanie do kolejki komunikatow
    int msgIdNarciarz = msgget(keyWyciag, 0);
    if (msgIdNarciarz == -1) blad("narciarz msgget narciarz");

    if (zjazdy== 100){
        cout << "[Narciarz #" << (nId+1) << "] START z biletem calodniowym" << endl;
    }
    else if(zjazdy == 1000){
        cout << "[Narciarz #" << (nId+1) << "] START z biletem VIP\033[33m [V.I.P]\033[0m" << endl;
    }
    else{
        cout << "[Narciarz #" << (nId+1) << "] START z " << zjazdy << " zjazdami" << endl;
    }


    int ileDzieci=0;
    if ((rand()%100)+1<=SZANSA_NA_POSIADANIE_DZIECKA) ileDzieci++;
    if ((rand()%100)+1<=SZANSA_NA_POSIADANIE_DZIECKA) ileDzieci++;
    sleep(rand() %2);
    while(zjazdy > 0) {

        sem_P(semIdBramkiWejscie);
        sem_P(semIdBramki);
        if (infoStacja->koniecSymulacji)
            {
            cout << "\033[37m[Narciarz #" << (nId+1) << "] Bramki nie dzialaja, ide do domu KONIEC\033[0m" << endl;
            sem_V(semIdBramki);
            sem_V(semIdBramkiWejscie);
            shmdt(infoStacja);
            shmdt(infoBramki);
            return 0;
            }
        if (iloscMiejscKolejki(infoBramki)<1+ileDzieci){
            sem_V(semIdBramki);
            sem_V(semIdBramkiWejscie);
            cout << "\033[37m[Narciarz #" << (nId+1) << "] Kolejka pelna, próbuje znowu za 5s\033[0m" << endl;
            sleep(5);
            continue;
        }
        if (zjazdy==100){
            for (int i=0; i<1+ileDzieci; i++) pushNarciarz(infoBramki,nId);
            if (ileDzieci>0){
                cout << "\033[37m[Narciarz #" << (nId+1) << "] Przeszedlem przez bramke do kolejki z dziecmi w liczbie: "<< ileDzieci <<"\033[0m" << endl;
            }
            else{
                cout << "\033[37m[Narciarz #" << (nId+1) << "] Przeszedlem przez bramke do kolejki\033[0m" << endl;
            }

        }
        else if(zjazdy==1000){
            for (int i=0; i<1+ileDzieci; i++) pushVipNarciarz(infoBramki,nId);
            if (ileDzieci>0){
                cout << "\033[37m[Narciarz #" << (nId+1) << "] Przeszedlem przez bramke do kolejki z dziecmi w liczbie: "<< ileDzieci <<"\033[33m [V.I.P]\033[0m" << endl;
            }
            else
            {
                cout << "\033[37m[Narciarz #" << (nId+1) << "] Przeszedlem przez bramke do kolejki\033[33m [V.I.P]\033[0m" << endl;
            }
        }
        else{
            for (int i=0; i<1+ileDzieci; i++) pushNarciarz(infoBramki,nId);
            zjazdy--;
            if (ileDzieci>0){
                cout << "\033[37m[Narciarz #" << (nId+1) << "] Przeszedlem przez bramke do kolejki z dziecmi w liczbie: "<< ileDzieci <<"\033[0m" << endl;
            }
            else
            {
                cout << "\033[37m[Narciarz #" << (nId+1) << "] Przeszedlem przez bramke do kolejki\033[0m" << endl;
            }
        }

        sem_V(semIdBramki);
        sem_V(semIdBramkiWejscie);

        //oczekiwanie na wysiadaj
        msgNarciarz msg1;
        if (msgrcv(msgIdNarciarz, &msg1, sizeof(msgNarciarz)-sizeof(long), 1000+nId,0)==-1){
            blad("[Narciarz] msgrcv wysiadaj error");
        }

        int losowaTrasa = rand()%3;

        if (infoStacja->koniecSymulacji)
        {
            //symulacja ostatniego zjazdu
            cout << "\033[35m[Narciarz #" << (nId+1) << "] Wysiadam na gorze, i zjezdzam\033[0m" << endl;

            //zjazd
            sleep(mozliweTrasy[losowaTrasa]);

            cout << "\033[35m[Narciarz #" << (nId+1) << "] Zjechałem trasą " << losowaTrasa+1 << ", idę do domu KONIEC\033[0m" << endl;

            //odlaczenie pamieci
            shmdt(infoStacja);
            shmdt(infoBramki);
            return 0;
        }
        else{
            //symulacja zjazdu
            cout << "\033[35m[Narciarz #" << (nId+1) << "] Wysiadam na gorze, i zjezdzam\033[0m" << endl;

            //zjazd
            sleep(mozliweTrasy[losowaTrasa]);

            cout << "\033[35m[Narciarz #" << (nId+1) << "] Zjechalem trasa " << losowaTrasa+1 << ", wracam na stacje\033[0m" << endl;
            if (zjazdy==100 || zjazdy ==1000){
                cout << "\033[34m[Narciarz #" << (nId+1) << "] Zjezdzam do woli\033[0m" << endl;
            }
            else{
                cout << "\033[34m[Narciarz #" << (nId+1) << "] Pozostale zjazdy: " << zjazdy << "\033[0m" << endl;
            }

            //powrot na stacje
            sleep(10);
        }
    }

    cout << "[Narciarz #"<< (nId+1) << "] Skonczylem wszystkie zjazdy KONIEC" << endl;

    //odlaczenie pamieci
    shmdt(infoStacja);
    shmdt(infoBramki);
    return 0;
}
