#include "common.h"

int main(int argc, char* argv[])
{
    srand(time(nullptr) ^ getpid());

    //klucz ipc
    key_t key = ftok(SCIEZKA_KLUCZA, KLUCZ_PROJ);
    if (key == -1) blad("narciarz ftok");

    int shmId = shmget(key, sizeof(StacjaInfo), 0);
    if (shmId == -1) blad("narciarz shmget");
    StacjaInfo* info = (StacjaInfo*)shmat(shmId, nullptr, 0);
    if (info == (void*)-1) blad("narciarz shmat");

    int semId = semget(key, 1, 0);
    if (semId == -1) blad("narciarz semget");

    while (true){
        sem_P(semId);
        bool endSim = info->koniecSymulacji;
        sem_V(semId);

        if (endSim) {
            cout << "[Narciarz PID=" << getpid() << "] Koniec" << endl;;
            shmdt(info);
            return 0;
        }

        int randomTime = rand() % 5 + 1;
        sleep(randomTime);
        break;
    }

    sem_P(semId);
    info->liczbaNarciarzyWKolejce++;
    int n = info->liczbaNarciarzyWKolejce;
    sem_V(semId);

    //cout << "[Narciarz PID=" << getpid() << "] Ustawiam sie w kolejce. (razem=" << n << ")" << endl;

    shmdt(info);
    return 0;
}
