#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <errno.h>
#include <algorithm>
#include <signal.h>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>

using namespace std;

//                                  USTAWIENIA

//ilosc,czasy jazdy krzeselka i czestotliwosc z jaką są wypuszczane to integralna czesc systemu wiec nie zostaje dodana jako ustawienie poniewaz negatywnie wplywa na sens działania stacji

//ustawienia stacji
static const int LICZBA_DNI=1; //ilosc dni czasu wewnetrznego przez ktore symulacja bedzie dzialac
static const int GODZINA_OTWARCIA = 6;
static const int GODZINA_ZAMKNIECIA = 16;   // w zagarze jedna godzina odpowiada relanej minucie a minuta to prawdziwa sekunda

//ustawienia turystow/narciarzy
static const int ILOSC_TURYSTOW_NA_OTWARCIU = 200; // ilosc turystow pojawiajacych sie po paru sekunach przy kasie od otwarcia stacji
static const int CZESTOTLIWOSC_TURYSTOW =1; // co  ( 0 - CZESTOTLIWOSC_TURYSTOW ) sekund pojawia sie nowy
static const int SZANSA_NA_BYCIE_NARCIARZEM = 70; //w % od 0 do 100%
static const int SZANSA_NA_ZAKUP_BILETU_VIP = 1; //w % od 0 do 100%
static const int SZANSA_NA_POSIADANIE_DZIECKA = 20; //w % od 0 do 100%
//ustawienia kolejki do peronu
static const int MAX_DLUGOSC_KOLEJKI = 100;
//ustawienia krzeselek i pracownikow
static const int SZANSA_NA_AWARIE_KOLEJKI = 1;// w % od 0.1 do 100%

//                                  TABLICE ZASOBOW LOSOWYCH

int mozliweTrasy[3]={10,15,20}; //10,15,20 sekund, 3 rozne trasy

static const int wyborBiletu[7]={3,5,10,100,102,104,106}; //3,5,10 zjazdow i na caly dzien (karnet vip osobno)

//                                  DEFINICJE ZASOBOW

//definicja kluczy ipc

static const char* SCIEZKA_KLUCZA_STACJA = "./tmp/stacja.key";
static const int   KLUCZ_PROJ_STACJA     = 65;
static const char* SCIEZKA_KLUCZA_WYCIAG = "./tmp/wyciag.key";
static const int   KLUCZ_PROJ_WYCIAG     = 23;
static const char* SCIEZKA_KLUCZA_BRAMKI = "./tmp/bramki.key";
static const int   KLUCZ_PROJ_BRAMKI     = 19;
static const char* SCIEZKA_KLUCZA_KASJER = "./tmp/kasjer.key";
static const int   KLUCZ_PROJ_KASJER     = 48;
static const char* SCIEZKA_KLUCZA_ZEGAR = "./tmp/zegar.key";
static const int   KLUCZ_PROJ_ZEGAR      = 75;

//sciezka pliku

static const char* SCIEZKA_PLIKU_LOGI = "./tmp/logi.txt";

//implementacja jednolitego zarzadzania bledami

inline void blad(const char* txt) {
    perror(txt);
    _exit(1);
}

//struktura pamieci StacjaInfo

struct StacjaInfo {

    bool koniecSymulacji;
};

//struktura pamieci WyciagInfo

struct  WyciagInfo{

    int stanKrzesla[80];
    int ileOsobNaKrzesle[80];
    int liczbaNarciarzyWTrasie;
    int krzeslaWTrasie;
    pid_t pidKrzesel[80];

};

//struktura pamieci BramkiInfo i implementacja kolejki fifo

struct Narciarz{
    int narciarzId;
    int liczbadzieci;
};

struct BramkiInfo{

    int liczbaNarciarzyWKolejce;
    int przod;
    int tyl;
    Narciarz kolejka[MAX_DLUGOSC_KOLEJKI];
};

inline bool czyKolejkaPelna(const BramkiInfo* br) {
    return (br->liczbaNarciarzyWKolejce == MAX_DLUGOSC_KOLEJKI);
}

inline int iloscMiejscKolejki(const BramkiInfo* br) {
    return (MAX_DLUGOSC_KOLEJKI-br->liczbaNarciarzyWKolejce);
}

inline bool czyKolejkaPusta(const BramkiInfo* br) {
    return (br->liczbaNarciarzyWKolejce == 0);
}

inline void pushNarciarz(BramkiInfo* br, const Narciarz& narciarz) {
    br->kolejka[br->tyl] = narciarz;
    br->tyl = (br->tyl + 1) % MAX_DLUGOSC_KOLEJKI;
    br->liczbaNarciarzyWKolejce++;
}

inline Narciarz popNarciarz(BramkiInfo* br) {
    Narciarz narciarz = br->kolejka[ br->przod ];
    br->przod = (br->przod + 1) % MAX_DLUGOSC_KOLEJKI;
    br->liczbaNarciarzyWKolejce--;
    return narciarz;
}

inline void pushVipNarciarz(BramkiInfo* br, const Narciarz& vipNarciarz) {
    br->przod = (br->przod + MAX_DLUGOSC_KOLEJKI - 1) % MAX_DLUGOSC_KOLEJKI;
    br->kolejka[br->przod] = vipNarciarz;
    br->liczbaNarciarzyWKolejce++;
}

//struktura pamieci ZegarInfo

struct ZegarInfo{
    int godzina;
    int minuta;
};

//struktury wiadomosci

struct msgWyciag {
    long mtype;
    int nrKrzesla;
    int liczbaOsob;
    int idNarciarzyNaKrzesle[3];
};

struct msgNarciarz {
    long mtype;
    int narciarzId;
};

struct msgKasjer {
    long mtype;
    int turystaId;
    int liczbaZjazdow;
    int wiek;
};

//implementacja jednolitego korzystania z semaforow

union semun {
    int val;
    struct semid_ds* buf;
    unsigned short* array;
};

inline void sem_op(int semid, short num, short op, short flg=0) {
    struct sembuf sb;
    sb.sem_num = num;
    sb.sem_op  = op;
    sb.sem_flg = flg;
    if (semop(semid, &sb, 1) == -1) {
        blad("semop");
        _exit(1);
    }
}
inline void sem_P(int semid, short num=0) { sem_op(semid, num, -1);}// blokowanie
inline void sem_V(int semid, short num=0) { sem_op(semid, num, +1);}// odblokowanie

#endif
