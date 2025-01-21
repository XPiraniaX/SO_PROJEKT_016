
#include "common.h"

int main()
{
    //klucze ipc
    key_t keyStacja = ftok(SCIEZKA_KLUCZA_STACJA, KLUCZ_PROJ_STACJA);
    if (keyStacja == -1) blad("kasjer ftok stacja");

    key_t keyKasjer = ftok(SCIEZKA_KLUCZA_KASJER, KLUCZ_PROJ_KASJER);
    if (keyKasjer == -1) blad("kasjer ftok kasjer");

    //dolaczanie do pamieci dzielonej
    int shmIdStacja = shmget(keyStacja, sizeof(StacjaInfo), 0);
    if (shmIdStacja == -1) blad("kasjer shmget stacja");
    StacjaInfo* infoStacja = (StacjaInfo*)shmat(shmIdStacja, nullptr, 0);
    if (infoStacja == (void*)-1) blad("kasjer shmat stacja");

    //podlaczanie do semafora
    int semIdStacja = semget(keyStacja, 1, 0);
    if (semIdStacja == -1) blad("kasjer semget stacja");

    //dolaczanie do kolejki komunikatow
    int msgIdKasjer = msgget(keyKasjer, 0);
    if (msgIdKasjer == -1) blad("kasjer msgget kasjer");

    cout << "[Kasjer] START"<<endl;

    while(true) {
        msgKasjer req;
        if (msgrcv(msgIdKasjer, &req, sizeof(req) - sizeof(long), -99, 0) == -1) {
            if (errno == EINTR) continue;
            blad("[Kasjer] msgrcv turysta error");
            break;
        }
        //sprawdzanie ilosci wiadomosci na msgIdKasjer
        /*struct msqid_ds buf;
        if (msgctl(msgIdKasjer, IPC_STAT, &buf) == -1) {
            perror("msgctl error");
        } else {
            cout << "Aktualna liczba wiadomości w kolejce: " << buf.msg_qnum <<", rozmiar wiadomosci " << sizeof(msgKasjer) << endl;

            cout << "Maksymalna pojemność kolejki: " << buf.msg_qbytes << " bajtów" << endl;
        }*/
        if (req.mtype == 99){
            break;
        }
        else{
            usleep(100000);

            //wysylamy bilet do turysty
            msgKasjer resp;
            resp.mtype = 2;
            resp.liczbaZjazdow = req.liczbaZjazdow;
            if (msgsnd(msgIdKasjer, &resp, sizeof(resp) - sizeof(long), IPC_NOWAIT)==-1){
                blad("[Kasjer] msgsnd tursta error");
                return 1;
            }

            cout << "[Kasjer] Sprzedano bilet na 3 zjazdy" << endl;
        }
    }
    cout << "[Kasjer] Zamykam kasy KONIEC"<<endl;
    shmdt(infoStacja);
    return 0;
}
