#include "common.h"

int main()
{
    //klucze ipc
    key_t keyKasjer = ftok(SCIEZKA_KLUCZA_KASJER, KLUCZ_PROJ_KASJER);
    if (keyKasjer == -1) blad("kasjer ftok kasjer");

    //podlaczanie do semafora
    int semIdKasjer = semget(keyKasjer, 1, 0);
    if (semIdKasjer == -1) blad("kasjer semget kasjer");

    //dolaczanie do kolejki komunikatow
    int msgIdKasjer = msgget(keyKasjer, 0);
    if (msgIdKasjer == -1) blad("kasjer msgget kasjer");

    cout << "[Kasjer] START"<<endl;

    while(true) {
        //oczekiwanie na turyste
        msgKasjer req;
        if (msgrcv(msgIdKasjer, &req, sizeof(req) - sizeof(long), 1, 0) == -1) {
            if (errno == EINTR) continue;
            blad("[Kasjer] msgrcv turysta error");
            break;
        }
        //infoStacja->koniecSymulacji //TODO

        //wysylamy bilet do turysty
        msgKasjer resp;
        resp.mtype = 2;
        resp.liczbaZjazdow = 3;
        if (msgsnd(msgIdKasjer, &resp, sizeof(resp) - sizeof(long), 0)==-1){
            blad("[Kasjer] msgsnd tursta error");
            return 1;
        }

        cout << "[Kasjer] Sprzedano bilet na 3 zjazdy" << endl;
    }

    cout << "[Kasjer] KONIEC"<<endl;
    return 0;
}
