#include "skier.h"
#include <iostream>
#define CHILD_PROBABILITYPROC 25
#define CHILD_PROBABILITYPROC2 5

int childNumber() {
    if (CHILD_PROBABILITYPROC + CHILD_PROBABILITYPROC2 > 100) {
        exit(1);                                                       //TODO ERROR
    }

    int random = rand() % 100;
    if (random < CHILD_PROBABILITYPROC) {
        return 1;
    }
    else if (random < CHILD_PROBABILITYPROC+CHILD_PROBABILITYPROC2) {
        return 2;
    }
    else {
        return 0;
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
