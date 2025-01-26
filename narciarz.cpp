#include "common.h"

int godzinakarnetu=0;
int minutakarnetu=0;

//funkcja zapisujaca
void zapiszLog(int godzina,int minuta,int narciarzId,int typKarnetu){

    int fd = open(SCIEZKA_PLIKU_LOGI,O_WRONLY|O_APPEND);
    if (fd ==- 1) blad("narciarz zapis bramek log");

    string log;

    if (godzina<10 && minuta>10){
        if (typKarnetu==1){
            log = "[Bramki] 0" + to_string(godzina) + ":" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu z liczba zjadow\n";
        }
        else if (typKarnetu==2){
            log = "[Bramki] 0" + to_string(godzina) + ":" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu dziennego\n";
        }
        else if (typKarnetu==3){
            log = "[Bramki] 0" + to_string(godzina) + ":" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu VIP [V.I.P]\n";
        }
        else{
            log = "[Bramki] 0" + to_string(godzina) + ":" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu godzinnego\n";
        }
    }
    else if (godzina>10 && minuta<10){
        if (typKarnetu==1){
            log = "[Bramki] " + to_string(godzina) + ":0" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu z liczba zjadow\n";
        }
        else if (typKarnetu==2){
            log = "[Bramki] " + to_string(godzina) + ":0" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu dziennego\n";
        }
        else if (typKarnetu==3){
            log = "[Bramki] " + to_string(godzina) + ":0" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu VIP [V.I.P]\n";
        }
        else{
            log = "[Bramki] " + to_string(godzina) + ":0" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu godzinnego\n";
        }
    }
    else if (godzina<10 && minuta<10){
        if (typKarnetu==1){
            log = "[Bramki] 0" + to_string(godzina) + ":0" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu z liczba zjadow\n";
        }
        else if (typKarnetu==2){
            log = "[Bramki] 0" + to_string(godzina) + ":0" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu dziennego\n";
        }
        else if (typKarnetu==3){
            log = "[Bramki] 0" + to_string(godzina) + ":0" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu VIP [V.I.P]\n";
        }
        else{
            log = "[Bramki] 0" + to_string(godzina) + ":0" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu godzinnego\n";
        }
    }
    else if (godzina<10 && minuta==10){
        if (typKarnetu==1){
            log = "[Bramki] 0" + to_string(godzina) + ":" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu z liczba zjadow\n";
        }
        else if (typKarnetu==2){
            log = "[Bramki] 0" + to_string(godzina) + ":" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu dziennego\n";
        }
        else if (typKarnetu==3){
            log = "[Bramki] 0" + to_string(godzina) + ":" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu VIP [V.I.P]\n";
        }
        else{
            log = "[Bramki] 0" + to_string(godzina) + ":" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu godzinnego\n";
        }
    }
    else{
        if (typKarnetu==1){
            log = "[Bramki] " + to_string(godzina) + ":" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu z liczba zjadow\n";
        }
        else if (typKarnetu==2){
            log = "[Bramki] " + to_string(godzina) + ":" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu dziennego\n";
        }
        else if (typKarnetu==3){
            log = "[Bramki] " + to_string(godzina) + ":" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu VIP [V.I.P]\n";
        }
        else{
            log = "[Bramki] " + to_string(godzina) + ":" + to_string(minuta) + " Narciarz #"+ to_string(narciarzId)+ " przeszedl przez bramki uzywajac karnetu godzinnego\n";
        }
    }

    ssize_t bytesWritten = write(fd,log.c_str(),log.size());
    if (bytesWritten == -1){blad("Zly zapis narciarz logi");}
    else if (static_cast<size_t>(bytesWritten) < log.size()){blad("Nie zapisano wszystkiego");}

    close(fd);
}

int main(int argc, char* argv[])
{
    //                                  WALIDACJA ARGUMENTOW

    if (argc < 3) {
        blad("[narciarz] Brak argumentow: liczba zjazdow/id narciarza!");
        return 1;
    }

    int zjazdy = atoi(argv[1]);
    int nId = atoi(argv[2]);

    srand(time(NULL)^getpid());

    //                                  INICJALIZACJA ZASOBOW

    //klucze ipc
    key_t keyStacja = ftok(SCIEZKA_KLUCZA_STACJA, KLUCZ_PROJ_STACJA);
    if (keyStacja == -1) blad("narciarz ftok stacja");

    key_t keyWyciag = ftok(SCIEZKA_KLUCZA_WYCIAG, KLUCZ_PROJ_WYCIAG);
    if (keyWyciag == -1) blad("narciarz ftok wyciag");

    key_t keyBramki = ftok(SCIEZKA_KLUCZA_BRAMKI, KLUCZ_PROJ_BRAMKI);
    if (keyBramki == -1) blad("narciarz ftok bramki");

    key_t keyZegar = ftok(SCIEZKA_KLUCZA_ZEGAR, KLUCZ_PROJ_KASJER);
    if (keyZegar == -1) blad("init ftok zegar");

    //dolaczanie do pamieci dzielonej
    int shmIdStacja = shmget(keyStacja, sizeof(StacjaInfo), 0);
    if (shmIdStacja == -1) blad("narciarz shmget stacja");
    StacjaInfo* infoStacja = (StacjaInfo*)shmat(shmIdStacja, nullptr, 0);
    if (infoStacja == (void*)-1) blad("narciarz shmat stacja");

    int shmIdBramki = shmget(keyBramki, sizeof(BramkiInfo), 0);
    if (shmIdBramki == -1) blad("narciarz shmget bramki");
    BramkiInfo* infoBramki = (BramkiInfo*)shmat(shmIdBramki, nullptr, 0);
    if (infoBramki == (void*)-1) blad("narciarz shmat bramki");

    int shmIdZegar = shmget(keyZegar, sizeof(ZegarInfo), 0);
    if (shmIdZegar == -1) blad("narciarz shmget zegar");
    ZegarInfo* infoZegar = (ZegarInfo*)shmat(shmIdZegar, nullptr, 0);
    if (infoZegar == (void*)-1) blad("narciarz shmat zegar");

    //podlaczanie do semafora

    int semIdBramki = semget(keyBramki, 1, 0);
    if (semIdBramki == -1) blad("narciarz semget bramki");

    int semIdBramkiWejscie = semget(keyBramki, 1, 0);
    if (semIdBramkiWejscie == -1) blad("narciarz semget bramki");

    int semIdPeronWyjazd = semget(keyWyciag, 1, 0);
    if (semIdPeronWyjazd == -1) blad("narciarz semget bramki");

    //dolaczanie do kolejki komunikatow
    int msgIdNarciarz = msgget(keyWyciag, 0);
    if (msgIdNarciarz == -1) blad("narciarz msgget narciarz");

    //                                  START SYMULACJI

    int typKarnetu=0;

    if (zjazdy== 100){
        cout << "[Narciarz #" << (nId+1) << "] START z biletem calodniowym" << endl;
        typKarnetu = 2;
    }
    else if(zjazdy >100 && zjazdy<=106){
        cout << "[Narciarz #" << (nId+1) << "] START z czasowym na " << zjazdy-100 << " godziny" << endl;
        typKarnetu = 4;
    }
    else if(zjazdy == 1000){
        cout << "[Narciarz #" << (nId+1) << "] START z biletem VIP\033[33m [V.I.P]\033[0m" << endl;
        typKarnetu = 3;
    }
    else{
        cout << "[Narciarz #" << (nId+1) << "] START z " << zjazdy << " zjazdami" << endl;
        typKarnetu = 1;
    }

    //losowanie czy narciarz posiada dzieci
    int ileDzieci=0;
    if ((rand()%100)+1<=SZANSA_NA_POSIADANIE_DZIECKA) ileDzieci++;
    if ((rand()%100)+1<=SZANSA_NA_POSIADANIE_DZIECKA) ileDzieci++;
    Narciarz narciarz = {nId,ileDzieci};

    //symulacja czasu dojscia do bramek
    sleep(rand() %2);

    godzinakarnetu=infoZegar->godzina;
    minutakarnetu=infoZegar->minuta;

    //narciarz sprawdza czy ma wazny karnet
    while(zjazdy > 0) {
        //zajmowanie bramki
        sem_P(semIdBramkiWejscie);
        sem_P(semIdBramki);
        //logika bramek

        if (infoStacja->koniecSymulacji)
        {
            cout << "\033[37m[Narciarz #" << (nId+1) << "] Bramki nie dzialaja, ide do domu KONIEC\033[0m" << endl;
            sem_V(semIdBramki);
            sem_V(semIdBramkiWejscie);
            shmdt(infoStacja);
            shmdt(infoBramki);
            shmdt(infoZegar);
            return 0;
        }
        else if (godzinakarnetu+zjazdy-100 >= infoZegar->godzina && minutakarnetu > infoZegar->minuta)
        {
            cout << "\033[37m[Narciarz #" << (nId+1) << "] Skonczyl mi sie bilet, ide do domu KONIEC\033[0m" << endl;
            sem_V(semIdBramki);
            sem_V(semIdBramkiWejscie);
            shmdt(infoStacja);
            shmdt(infoBramki);
            shmdt(infoZegar);
            return 0;
        }

        if (iloscMiejscKolejki(infoBramki)<narciarz.liczbadzieci+1){
            sem_V(semIdBramki);
            sem_V(semIdBramkiWejscie);
            cout << "\033[37m[Narciarz #" << (nId+1) << "] Kolejka pelna, próbuje znowu za 5s\033[0m" << endl;
            sleep(5);
            continue;
        }
        if (typKarnetu==2 || typKarnetu==4){
            pushNarciarz(infoBramki,narciarz);
            if (ileDzieci>0){
                cout << "\033[37m[Narciarz #" << (nId+1) << "] Przeszedlem przez bramke do kolejki z dziecmi w liczbie: "<< ileDzieci <<"\033[0m" << endl;
            }
            else{
                cout << "\033[37m[Narciarz #" << (nId+1) << "] Przeszedlem przez bramke do kolejki\033[0m" << endl;
            }

        }
        else if(typKarnetu==3){
            pushVipNarciarz(infoBramki,narciarz);
            if (ileDzieci>0){
                cout << "\033[37m[Narciarz #" << (nId+1) << "] Przeszedlem przez bramke do kolejki z dziecmi w liczbie: "<< ileDzieci <<"\033[33m [V.I.P]\033[0m" << endl;
            }
            else
            {
                cout << "\033[37m[Narciarz #" << (nId+1) << "] Przeszedlem przez bramke do kolejki\033[33m [V.I.P]\033[0m" << endl;
            }
        }
        else{
            pushNarciarz(infoBramki,narciarz);
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
        //zapis do bramek
        zapiszLog(infoZegar->godzina,infoZegar->minuta,nId+1,typKarnetu);

        //oczekiwanie na wysiadaj
        msgNarciarz msg1;
        if (msgrcv(msgIdNarciarz, &msg1, sizeof(msgNarciarz)-sizeof(long), 1000+nId,0)==-1){
            blad("[Narciarz] msgrcv wysiadaj error");
        }

        //symulacja 2 wyjazdow z peronu gora
        sem_P(semIdBramkiWejscie);
        sem_V(semIdBramkiWejscie);
        //losowanie trasy
        int losowaTrasa = rand()%3;

        //zjazd zalezny od tego czy stacja sie zamyka
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
            shmdt(infoZegar);
            return 0;
        }
        else{
            //symulacja zjazdu
            cout << "\033[35m[Narciarz #" << (nId+1) << "] Wysiadam na gorze, i zjezdzam\033[0m" << endl;

            //zjazd
            sleep(mozliweTrasy[losowaTrasa]);

            cout << "\033[35m[Narciarz #" << (nId+1) << "] Zjechalem trasa " << losowaTrasa+1 << ", wracam na stacje\033[0m" << endl;
            if (typKarnetu==1 || typKarnetu==2){
                cout << "\033[34m[Narciarz #" << (nId+1) << "] Zjezdzam do woli\033[0m" << endl;
            }
            else{
                cout << "\033[34m[Narciarz #" << (nId+1) << "] Pozostale zjazdy: " << zjazdy << "\033[0m" << endl;
            }

            //powrot na stacje
            sleep(10);
        }
    }
    //                                  KONIEC NARCIARZA
    cout << "[Narciarz #"<< (nId+1) << "] Skonczylem wszystkie zjazdy KONIEC" << endl;

    //odlaczenie pamieci
    shmdt(infoStacja);
    shmdt(infoBramki);
    shmdt(infoZegar);
    return 0;
}
