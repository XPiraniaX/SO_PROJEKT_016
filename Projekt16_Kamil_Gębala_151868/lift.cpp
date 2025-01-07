#include "Lift.h"
#include <iostream>
#include <thread>
#include <chrono>

Lift::Lift(int chair_count) : current_chair_index(0) {
    for (int i = 0; i < chair_count; ++i) {
        chairs.push_back(make_shared<Chair>(i));
    }
}

void Lift::startLift() {
    for (auto& chair : chairs) {
        thread(&Chair::startRide, chair).detach();
        this_thread::sleep_for(chrono::seconds(1)); 
    }
}

shared_ptr<Chair> Lift::getNextAvailableChair() {
    lock_guard<mutex> lock(lift_mutex);


    for (size_t i = 0; i < chairs.size(); ++i) {
        current_chair_index = (current_chair_index + 1) % chairs.size();
        if (!chairs[current_chair_index]->isFull()) {
            return chairs[current_chair_index];
        }
    }
    return nullptr; 
}
