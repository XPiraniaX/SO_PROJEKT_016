#include <iostream>
#include <thread>
#include <vector>
#include "lift.h"
#include "skier.h"

using namespace std;

void skierSimulation(Lift& lift, Skier& skier) {
    if (lift.canLoadMore()) {   
        if (lift.loadChair(skier)) {  
            this_thread::sleep_for(chrono::seconds(2));   
        }
    }
    else {
        cout << "Nie mozna zaladowac narciarza " << skier.getId() << ". Brak miejsca na kolejce!" << endl;
    }
}

int main() {
    Lift lift(80, 40); 
    vector<Skier> skiers;


    for (int i = 0; i < 20; ++i) {
        skiers.push_back(Skier(i)); 
    }


    vector<thread> threads;
    for (auto& skier : skiers) {
        threads.push_back(thread(skierSimulation, ref(lift), ref(skier)));
    }


    for (auto& t : threads) {
        t.join();
    }


    lift.showLiftStatus();

    return 0;
}
