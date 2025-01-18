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

    int semIdKasjer = semget(keyKasjer, 1, 0);
    if (semIdKasjer == -1) blad("kasjer semget kasjer");

    //dolaczanie do kolejki komunikatow
    int msgIdKasjer = msgget(keyKasjer, 0);
    if (msgIdKasjer == -1) blad("kasjer msgget kasjer");

    cout << "[Kasjer] START"<<endl;

    while(true) {

        sem_P(semIdStacja);
        bool endSim = infoStacja->koniecSymulacji;
        sem_V(semIdStacja);

        if (endSim){
            cout << "[Kasjer] KONIEC" << endl;
            break;
        }

        //oczekiwanie na turyste
        msgKasjer req;
        if (msgrcv(msgIdKasjer, &req, sizeof(req) - sizeof(long), 0, IPC_NOWAIT) == -1) {
            if (errno == ENOMSG)
            {
                sem_P(semIdStacja);
                bool endSim = infoStacja->koniecSymulacji;
                sem_V(semIdStacja);

                if (endSim){
                    cout << "[Kasjer] KONIEC" << endl;
                    break;
                }
                continue;
            }
            else if (errno == EINTR){
                continue;
            }
            else{
                blad("[Kasjer] msgrcv turysta error");
                break;
            }
        }


        sleep(1);

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

    shmdt(infoStacja);
    return 0;
}
