#include <iostream>
#include <thread>
#include <vector>
#include "lift.h"
#include "skier.h"

using namespace std;

void skierSimulation(Lift& lift, Skier& skier) {
    if (lift.canLoadMore()) {   // Sprawdzamy, czy mo¿na za³adowaæ narciarza na kolejkê
        if (lift.loadChair(skier)) {  // Próba za³adowania narciarza na kolejkê
            this_thread::sleep_for(chrono::seconds(2));  // Symulacja czasu przejazdu
            lift.unloadChair(0);  // Wysadzenie narciarzy z pierwszego krzese³ka
        }
    }
    else {
        cout << "Nie mozna zaladowac narciarza " << skier.getId() << ". Brak miejsca na kolejce!" << endl;
    }
}

int main() {
    Lift lift(80, 40);  // Kolej linowa o maksymalnej liczbie krzese³ek 80, ale tylko 40 zajêtych jednoczeœnie
    vector<Skier> skiers;

    // Tworzymy narciarzy
    for (int i = 0; i < 20; ++i) {
        skiers.push_back(Skier(i));  // Ka¿dy narciarz jest osobnym obiektem
    }

    // Symulujemy wsiadanie narciarzy na kolejkê
    vector<thread> threads;
    for (auto& skier : skiers) {
        threads.push_back(thread(skierSimulation, ref(lift), ref(skier)));
    }

    // Czekamy na zakoñczenie wszystkich w¹tków
    for (auto& t : threads) {
        t.join();
    }

    // Pokazujemy status kolejki po zakoñczeniu
    lift.showLiftStatus();

    return 0;
}
