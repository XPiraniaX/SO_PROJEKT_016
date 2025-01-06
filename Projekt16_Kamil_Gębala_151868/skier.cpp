#include "skier.h"

#include <iostream>

Skier::Skier(int id) : id(id) {}

int Skier::getId() const {
    return id;
}
