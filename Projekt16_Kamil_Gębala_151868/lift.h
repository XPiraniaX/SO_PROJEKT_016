#ifndef LIFT_H
#define LIFT_H

#include <vector>
#include <thread>
#include "Chair.h"

using namespace std;

class Lift {
private:
    vector<shared_ptr<Chair>> chairs;
    int current_chair_index;
    mutable mutex lift_mutex;

public:
    Lift(int chair_count);
    void startLift();
    shared_ptr<Chair> getNextAvailableChair();

};

#endif 
