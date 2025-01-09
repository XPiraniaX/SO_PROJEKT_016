#include "SkiStation.h"
#include <iostream>
#include <ctime>

int main() {
    srand(time(0));
    const int skier_count = 10;    
    

    std::cout << "Rozpoczynamy symulacje stacji narciarskiej" << std::endl;

    SkiStation station(skier_count);
    station.startSimulation();

    return 0;
}