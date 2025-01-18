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

using namespace std;

//definicja kluczy ipc

static const char* SCIEZKA_KLUCZA_STACJA = "./tmp/stacja";
static const int   KLUCZ_PROJ_STACJA     = 65;
static const char* SCIEZKA_KLUCZA_WYCIAG = "./tmp/wyciag";
static const int   KLUCZ_PROJ_WYCIAG     = 23;
static const char* SCIEZKA_KLUCZA_BRAMKI = "./tmp/bramki";
static const int   KLUCZ_PROJ_BRAMKI     = 19;
static const char* SCIEZKA_KLUCZA_KASJER = "./tmp/kasjer";
static const int   KLUCZ_PROJ_KASJER     = 48;

//ustawienia stacji

static const int DLUGOSC_SYMULACJI = 20;
static const int ILOSC_TURYSTOW_NA_OTWARCIU = 20;
static const int CZESTOTLIWOSC_TURYSTOW = 5; // co  ( 0 - CZESTOTLIWOSC_TURYSTOW ) sekund pojawia sie nowy
static const int MAX_DLUGOSC_KOLEJKI = 100;

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

};

//struktura pamieci BramkiInfo i implementacja kolejki fifo

struct BramkiInfo{

    int liczbaNarciarzyWKolejce;
    int przod;
    int tyl;
    int kolejka[MAX_DLUGOSC_KOLEJKI];
};

inline bool czyKolejkaPelna(const BramkiInfo* br) {
    return (br->liczbaNarciarzyWKolejce == MAX_DLUGOSC_KOLEJKI);
}

inline bool czyKolejkaPusta(const BramkiInfo* br) {
    return (br->liczbaNarciarzyWKolejce == 0);
}

inline void pushNarciarz(BramkiInfo* br, int nId) {
    br->kolejka[br->tyl] = nId;
    br->tyl = (br->tyl + 1) % MAX_DLUGOSC_KOLEJKI;
    br->liczbaNarciarzyWKolejce++;
}

inline int popNarciarz(BramkiInfo* br) {
    int nId = br->kolejka[ br->przod ];
    br->przod = (br->przod + 1) % MAX_DLUGOSC_KOLEJKI;
    br->liczbaNarciarzyWKolejce--;
    return nId;
}

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
    int liczbaZjazdow;
};

//implementacja jednolitego zarzadzania semaforami

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
inline void sem_P(int semid, short num=0) { sem_op(semid, num, -1);}
inline void sem_V(int semid, short num=0) { sem_op(semid, num, +1);}

#endif
