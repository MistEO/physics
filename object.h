#pragma once

#include <utility>
#include <vector>

#include "define.h"

class Time;
class Space;

class Object {
    friend class Time;
    friend class Space;

public:
    Object(double mass, Coordinate coordinate = Coordinate(0, 0), Velocity velocity = Velocity(0, 0));

    Force sum_of_forces() const;
    Coordinate will_go(double time = 1.0) const;

    const double mass = 1.0;
    Coordinate coordinate;
    Velocity velocity;
    std::vector<Force*> forces;
    double elasticity = 1.0;
    double friction = 0;

private:
    void tick(double time = 1.0);
};
