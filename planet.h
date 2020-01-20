#pragma once

#include "object.h"
#include <vector>

class Planet : public Object {
public:
    Planet(double mass, Coordinate coordinate = Coordinate(0, 0), Velocity velocity = Velocity(0, 0));
    std::vector<std::pair<Object*, Force*>> gravity_pair_vector;
    Force sum_of_forces() const override;
};