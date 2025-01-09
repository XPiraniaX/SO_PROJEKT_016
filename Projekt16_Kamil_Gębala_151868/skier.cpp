#include "skier.h"
#include <iostream>
#define CHILD_PROBABILITYPROC 25
#define CHILD_PROBABILITYPROC2 5

int childNumber() {
    int random = rand() % 100;
    if (random < 100-CHILD_PROBABILITYPROC+CHILD_PROBABILITYPROC2) {
        return 0;
    }
    else if (random < 100 - CHILD_PROBABILITYPROC2) {
        return 1;
    }
    else {
        return 2;
    }
}

Skier::Skier(int id) : id(id) {
    num_children = childNumber();
}

int Skier::getId() const {
    return id;
}

int Skier::getChildren() const {
    return num_children;
}
