#include "Chair.h"
#include <iostream>
#include <thread>
#include <chrono>

Chair::Chair(int chair_id) : id(chair_id), is_full(false) {}

bool Chair::loadSkier(Skier& skier) {
    int persons = skier.getChildren();
    if (skier.getChildren() > 2) {
        cout << "Blad" << endl;              /////////// TODO ERROR
        exit(0);
    }
    lock_guard<mutex> lock(chair_mutex);
    if (passengers.size()+persons < 3) {
        for (int j = 0; j <= persons; j++) passengers.push_back(skier);
        if (passengers.size() == 3) {
            is_full = true;
        }
        return true;
    }
    return false;
}

void Chair::unloadSkier() {
    lock_guard<mutex> lock(chair_mutex);
    passengers.clear();
    is_full = false;
}

void Chair::startRide() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(40)); 
        unloadSkier();
        //cout << "Krzeselko " << id << " dojechalo na gore i oprozniono narciarzy." << endl;

        this_thread::sleep_for(chrono::seconds(40)); 
        //cout << "Krzeselko " << id << " wrocilo na dol i jest gotowe na nowa runde." << endl;
    }
}

bool Chair::isFull() const {
    return is_full;
}

int Chair::getId() const {
    return id;
}
