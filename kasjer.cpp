#include "common.h"

int main()
{
    //                                  INICJALIZACJA ZASOBOW

    //klucze ipc
    key_t keyKasjer = ftok(SCIEZKA_KLUCZA_KASJER, KLUCZ_PROJ_KASJER);
    if (keyKasjer == -1) blad("kasjer ftok kasjer");

    //dolaczanie do kolejki komunikatow
    int msgIdKasjer = msgget(keyKasjer, 0);
    if (msgIdKasjer == -1) blad("kasjer msgget kasjer");

    //                                  START SYMULACJI

    cout << "\033[34m[Kasjer] START\033[0m"<<endl;

    srand(time(NULL)^getpid());

    while(true) {
        //rejestracja sygnalu od turysty/innit
        msgKasjer req;
        if (msgrcv(msgIdKasjer, &req, sizeof(msgKasjer) - sizeof(long), -99, 0) == -1) {
            if (errno == EINTR) continue;
            blad("[Kasjer] msgrcv turysta error");
        }
        //sprawdzanie ilosci wiadomosci na msgIdKasjer
        /*struct msqid_ds buf;
        if (msgctl(msgIdKasjer, IPC_STAT, &buf) == -1) {
            perror("msgctl error");
        } else {
            cout << "Aktualna liczba wiadomości w kolejce: " << buf.msg_qnum <<", rozmiar wiadomosci " << sizeof(msgKasjer) << endl;

            cout << "Maksymalna pojemność kolejki: " << buf.msg_qbytes << " bajtów" << endl;
        }*/
        //otrzymano komuniakt o zakonczeniu dzialania
        if (req.mtype == 99){
            break;
        }
        //trzymano komunikat od turysty
        else{
            usleep(100000);

            //wysylamy bilet do turysty
            msgKasjer resp;
            resp.mtype = req.turystaId;
            resp.liczbaZjazdow = req.liczbaZjazdow;
            resp.wiek=0;
            resp.turystaId=0;
            if (msgsnd(msgIdKasjer, &resp, sizeof(msgKasjer) - sizeof(long), IPC_NOWAIT)==-1){
                blad("[Kasjer] msgsnd tursta error");
            }
            if (resp.liczbaZjazdow == 100){
                cout << "\033[34m[Kasjer] Sprzedano bilet calodniowy\033[0m" << endl;
            }
            else if (resp.liczbaZjazdow == 1000){
                cout << "\033[34m[Kasjer] Sprzedano bilet VIP\033[33m [V.I.P]\033[0m" << endl;
            }
            else{
                cout << "\033[34m[Kasjer] Sprzedano bilet uprawniajacy do "<< resp.liczbaZjazdow << " zjazdow\033[0m" << endl;
            }
            if (req.wiek < 8 || req.wiek >65){
                cout << "\033[34m[Kasjer] Przyznano znizke 25%\033[0m" << endl;
            }
        }
    }
    //                                  KONIEC SYMULACJI
    cout << "\033[34m[Kasjer] Zamykam kasy KONIEC\033[0m"<<endl;
    return 0;
}
