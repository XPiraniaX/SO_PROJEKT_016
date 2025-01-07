#ifndef SKISTATION_H
#define SKISTATION_H

#include "Lift.h"
#include "Skier.h"
#include <vector>

using namespace std;

class SkiStation {
public:
    SkiStation(int skier_count, int chair_count);
    void startSimulation();

private:
    std::vector<Skier> skiers;
    std::shared_ptr<Lift> lift;
};

#endif 

