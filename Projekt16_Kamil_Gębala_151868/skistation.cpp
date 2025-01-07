#include "SkiStation.h"
#include <iostream>
#include <thread>

SkiStation::SkiStation(int skier_count, int chair_count)
    : lift(make_shared<Lift>(chair_count)) {
    for (int i = 0; i < skier_count; ++i) {
        skiers.emplace_back(i);
    }
}

void SkiStation::startSimulation() {
    lift->startLift();

    size_t skier_index = 0;

    while (skier_index < skiers.size()) {
        shared_ptr<Chair> chair = lift->getNextAvailableChair();

        if (chair) {
            bool loaded = true;
            for (int i = 0; i < 3; ++i) { 
                if (skier_index < skiers.size()) {
                    if (!chair->loadSkier(skiers[skier_index])) {
                        loaded = false;
                        break;
                    }
                    cout << "Narciarz " << skiers[skier_index].getId()
                        << " wsiadl na krzeselko " << chair->getId() << "." << endl;
                    ++skier_index;
                }
                else {
                    break;
                }
            }

            if (loaded) {
                this_thread::sleep_for(chrono::seconds(1)); 
            }
        }
        else {
            this_thread::sleep_for(chrono::milliseconds(100)); 
        }
    }
}
