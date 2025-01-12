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

using namespace std;

static const char* SCIEZKA_KLUCZA = "./tmp";
static const int   KLUCZ_PROJ     = 65;

static const int LICZBA_NARCIARZY   = 20;


struct StacjaInfo {

    int krzeslaWTrasie;

    int liczbaNarciarzyWKolejce;

    int liczbaNarciarzyWTrasie;

    int stanKrzesla[80];

    int ileOsobNaKrzesle[80];
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
        perror("semop");
        _exit(1);
    }
}
inline void sem_P(int semid, short num=0) { sem_op(semid, num, -1); }
inline void sem_V(int semid, short num=0) { sem_op(semid, num, +1); }

inline void blad(const char* txt) {
    perror(txt);
    _exit(1);
}

#endif
