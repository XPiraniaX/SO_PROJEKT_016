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

static const char* SCIEZKA_KLUCZA_STACJA = "./tmp/stacja";
static const int   KLUCZ_PROJ_STACJA     = 65;
static const char* SCIEZKA_KLUCZA_WYCIAG = "./tmp/wyciag";
static const int   KLUCZ_PROJ_WYCIAG     = 23;
static const char* SCIEZKA_KLUCZA_BRAMKI = "./tmp/bramki";
static const int   KLUCZ_PROJ_BRAMKI     = 19;

static const int LICZBA_NARCIARZY   = 20;
static const int DLUGOSC_SYMULACJI = 20;

inline void blad(const char* txt) {
    perror(txt);
    _exit(1);
}

struct StacjaInfo {

    bool koniecSymulacji;
};

struct  WyciagInfo{

    int stanKrzesla[80];
    int ileOsobNaKrzesle[80];
    int liczbaNarciarzyWTrasie;
    int krzeslaWTrasie;

};

struct BramkiInfo{

    int liczbaNarciarzyWKolejce;
};

struct Komunikat {
    long mtype;
    int nrKrzesla;
    int liczbaOsob;
};


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
