#ifndef CHAIR_H
#define CHAIR_H

#include <vector>
#include <mutex>
#include "Skier.h"

using namespace std;

class Chair {
private:
    int id;
    vector<Skier> passengers;
    bool is_full;
    mutable mutex chair_mutex;


public:
    Chair(int chair_id);
    Chair(const Chair&) = delete;
    Chair& operator=(const Chair&) = delete;

    bool loadSkier(Skier& skier);
    void unloadSkier();
    void startRide();
    bool isFull() const;
    int getId() const;
};


#endif 

