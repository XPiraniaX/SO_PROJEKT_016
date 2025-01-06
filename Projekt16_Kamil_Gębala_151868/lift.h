#ifndef LIFT_H
#define LIFT_H

#include <vector>
#include <mutex>
#include "skier.h"

using namespace std;

class Lift {
private:
    int max_chairs;       
    int max_occupied_chairs; 
    int occupied_chairs; 
    vector<vector<Skier>> chairs;
    mutex lift_mutex; 

public:
    Lift(int max_chairs, int max_occupied_chairs); 
    bool loadChair(Skier& skier); 
    void unloadChair(int chairIndex); 
    int getOccupiedChairs() const; 
    bool canLoadMore() const;
    void showLiftStatus();
};

#endif
