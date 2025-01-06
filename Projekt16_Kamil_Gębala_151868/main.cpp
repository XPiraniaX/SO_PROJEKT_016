#include <iostream>
#include <thread>
#include <vector>
#include "lift.h"
#include "skier.h"

using namespace std;

void skierSimulation(Lift& lift, Skier& skier) {
    if (lift.canLoadMore()) {   // Sprawdzamy, czy mo�na za�adowa� narciarza na kolejk�
        if (lift.loadChair(skier)) {  // Pr�ba za�adowania narciarza na kolejk�
            this_thread::sleep_for(chrono::seconds(2));  // Symulacja czasu przejazdu
            lift.unloadChair(0);  // Wysadzenie narciarzy z pierwszego krzese�ka
        }
    }
    else {
        cout << "Nie mozna zaladowac narciarza " << skier.getId() << ". Brak miejsca na kolejce!" << endl;
    }
}

int main() {
    Lift lift(80, 40);  // Kolej linowa o maksymalnej liczbie krzese�ek 80, ale tylko 40 zaj�tych jednocze�nie
    vector<Skier> skiers;

    // Tworzymy narciarzy
    for (int i = 0; i < 20; ++i) {
        skiers.push_back(Skier(i));  // Ka�dy narciarz jest osobnym obiektem
    }

    // Symulujemy wsiadanie narciarzy na kolejk�
    vector<thread> threads;
    for (auto& skier : skiers) {
        threads.push_back(thread(skierSimulation, ref(lift), ref(skier)));
    }

    // Czekamy na zako�czenie wszystkich w�tk�w
    for (auto& t : threads) {
        t.join();
    }

    // Pokazujemy status kolejki po zako�czeniu
    lift.showLiftStatus();

    return 0;
}
