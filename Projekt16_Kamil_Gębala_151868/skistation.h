#ifndef SKISTATION_H
#define SKISTATION_H

#include "Lift.h"
#include "Skier.h"
#include <vector>

using namespace std;

class SkiStation {
private:
    vector<Skier> skiers;
    shared_ptr<Lift> lift;

public:
    SkiStation(int skier_count);
    void startSimulation();
};

#endif 

