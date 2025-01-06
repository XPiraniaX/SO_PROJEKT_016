#include "lift.h"
#include <iostream>

mutex cout_mutex;

Lift::Lift(int max_chairs, int max_occupied_chairs)
    : max_chairs(max_chairs), max_occupied_chairs(max_occupied_chairs), occupied_chairs(0) {
    chairs.resize(max_chairs); 
}

bool Lift::loadChair(Skier& skier) {
    lock_guard<mutex> lock(lift_mutex);  

    for (int i = 0; i < max_chairs; ++i) {
        if (chairs[i].size() < 3) {  
            chairs[i].push_back(skier);
            {
                lock_guard<mutex> cout_lock(cout_mutex); 
                cout << "Narciarz " << skier.getId() << " wsiadl na krzeselko " << i << "." << endl;
            }
            return true;
        }
    }
    {
        lock_guard<mutex> cout_lock(cout_mutex);
        cout << "Brak miejsca na kolejce dla narciarza " << skier.getId() << "." << endl;
    }
    return false;
}

void Lift::unloadChair(int chairIndex) {
    lock_guard<mutex> lock(lift_mutex);

    if (!chairs[chairIndex].empty()) {
        cout << "Wysadzono narciarzy z krzeselka " << chairIndex << "." << endl;
        occupied_chairs -= chairs[chairIndex].size();
        chairs[chairIndex].clear(); 
    }
}

int Lift::getOccupiedChairs() const {
    return occupied_chairs;
}

bool Lift::canLoadMore() const {
    return (occupied_chairs < max_occupied_chairs);
}

void Lift::showLiftStatus() {
    lock_guard<mutex> lock(lift_mutex);
    for (int i = 0; i < max_chairs; ++i) {
        cout << "Krzeselko " << i << ": " << chairs[i].size() << " narciarzy" << endl;
    }
}
