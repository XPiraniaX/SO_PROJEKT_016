#include "skier.h"

#include <iostream>

Skier::Skier(int id,int num_children) : id(id), num_children(num_children) {}

int Skier::getId() const {
    return id;
}

int Skier::getChildren() const {
    return num_children;
}
