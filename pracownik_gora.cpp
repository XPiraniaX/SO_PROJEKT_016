#include <variant>

#include "common.h"

int main()
{
    //                                  INICJALIZACJA ZASOBOW

    //klucze ipc
    key_t keyWyciag = ftok(SCIEZKA_KLUCZA_WYCIAG, KLUCZ_PROJ_WYCIAG);
    if (keyWyciag == -1) blad("pracownik_dol ftok wyciag");

    int shmIdWyciag = shmget(keyWyciag, sizeof(WyciagInfo), 0);
    if (shmIdWyciag == -1) blad("pracownik_gora shmget wyciag");
    WyciagInfo* infoWyciag = (WyciagInfo*)shmat(shmIdWyciag, nullptr, 0);
    if (infoWyciag == (void*)-1) blad("pracownik_gora shmat wyciag");

    //podlaczanie do semafora
    int semIdWyciag = semget(keyWyciag, 1, 0);
    if (semIdWyciag == -1) blad("pracownik_gora semget wyciag");

    //dolaczanie do kolejki komunikatow
    int msgIdWyciag = msgget(keyWyciag, 0);
    if (msgIdWyciag == -1) blad("pracownik_gora msgget wyciag");

    int msgIdNarciarz = msgget(keyWyciag, 0);
    if (msgIdNarciarz == -1) blad("pracownik_gora msgget narciarz");

    //                                  START SYMULACJI

    cout << "\033[32m[Pracownik Gorna Stacja] START\033[0m" << endl;

    while(true) {
        sleep(1);

        //otrzymanie komunikatu o koncu
        msgWyciag koniec;
        if (msgrcv(msgIdWyciag, &koniec, sizeof(koniec)-sizeof(long), 100000, IPC_NOWAIT) != -1){
            sleep(5);
            break;
        }
        else if (errno ==ENOMSG){}
        else{
            blad("[Pracownik Gorna Stacja] msgrcv pracownik_dol error");
        }

        //oczekiwanie na komunikat krzesla/pracownika_dol
        msgWyciag msg1;
        msg1.mtype = -1;
        if (msgrcv(msgIdWyciag, &msg1, sizeof(msgWyciag)-sizeof(long), -299, 0) == -1) {
            blad("[Pracownik Gorna Stacja] msgrcv krzeslo error");
        }

        if (msg1.mtype==290){ //otrzymanie komunikatu o awarii
            cout << "\033[32m[Pracownik Gorna Stacja] Awaria krzeselek, przerwa techniczna \033[31m[AWARIA]\033[0m"<< endl;

            //naprawa awarii
            sleep(2);

            //komunikat o naprawie do pracownika dol
            msgWyciag naprawaPracownikDol;
            naprawaPracownikDol.mtype=291;
            if (msgsnd(msgIdWyciag, &naprawaPracownikDol, sizeof(naprawaPracownikDol) - sizeof(long), 0) == -1) {
                blad("[INIT] msgsnd pracownicy naprawa error");
            }

            cout << "\033[32m[Pracownik Gorna Stacja] Awaria zostala naprawiona, wznawiamy dzialanie krzeselek \033[31m[AWARIA]\033[0m"<< endl;
            continue;
        }
        sem_P(semIdWyciag);
        long baseType = msg1.mtype - 200;
        if (baseType >= 0 && baseType < 80) {
            int kId = msg1.nrKrzesla;
            int ile = msg1.liczbaOsob;

            infoWyciag->krzeslaWTrasie--;
            infoWyciag->liczbaNarciarzyWTrasie -= ile;
            cout << "\033[32m[Pracownik Gorna Stacja] Krzeslo #" << (kId+1) << " dotarło z " << ile << " osobami. wTrasie=" << infoWyciag->krzeslaWTrasie << "\033[0m"<< endl;

            //wysiadanie narciarzy
            int ostanieId = -1;
            for (int i=0;i<ile;i++){
                int biezaceId = msg1.idNarciarzyNaKrzesle[i];

                if (biezaceId == ostanieId){
                    cout << "\033[32m[Pracownik Gorna Stacja] Dziecko Narciarza #" << msg1.idNarciarzyNaKrzesle[i] +1 << " wysiada\033[0m" << endl;
                    continue;
                }

                msgNarciarz msg2;
                msg2.mtype = 1000+msg1.idNarciarzyNaKrzesle[i];
                msg2.narciarzId = msg1.idNarciarzyNaKrzesle[i];
                if(msgsnd(msgIdNarciarz, &msg2, sizeof(msgNarciarz)-sizeof(long), 0) ==-1){
                    blad("[Pracownik Gorna Stacja] msgsnd narciarz error");
                }
                cout << "\033[32m[Pracownik Gorna Stacja] Narciarz #" << msg1.idNarciarzyNaKrzesle[i] +1 << " wysiada\033[0m" << endl;
                ostanieId=biezaceId;
            }
            sem_V(semIdWyciag);

            //odeslanie krzesla
            msgWyciag msg3;
            msg3.mtype = 300 + kId;
            msg3.nrKrzesla = kId;
            msg3.liczbaOsob= 0;
            memset(msg3.idNarciarzyNaKrzesle, 0, sizeof(msg3.idNarciarzyNaKrzesle));
            if (msgsnd(msgIdWyciag, &msg3, sizeof(msgWyciag)-sizeof(long), 0)==-1){
                blad("[Pracownik Gorna Stacja] msgsnd krzeslo error");
            }

        }
        else{
            sem_V(semIdWyciag);
        }
    }
    //                                  KONIEC SYMULACJI

    cout << "\033[32m[Pracownik Gorna Stacja] Stacja zamknieta i krzeselka puste KONIEC\033[0m" << endl;
    //odlaczenie pamieci
    shmdt(infoWyciag);
    return 0;
}
