#include "SkiStation.h"
#include <iostream>

int main() {
    const int skier_count = 100;    
    const int chair_count = 80;     

    std::cout << "Rozpoczynamy symulacje stacji narciarskiej" << std::endl;

    SkiStation station(skier_count, chair_count);
    station.startSimulation();

    return 0;
}