#pragma once

#include <utility>
#include <vector>

#include "define.h"

class Object {
public:
    Object(double mass, Coordinate coordinate = Coordinate(0, 0), Velocity velocity = Velocity(0, 0));

    void tick(double time = 1.0);
    void push_force(Force* force);
    virtual Force sum_of_forces() const;
    virtual Coordinate will_go(double time = 1.0) const;

    const double mass = 1.0;
    Coordinate coordinate;
    Velocity velocity;
    double elasticity = 1.0;
    double friction = 0;

private:
    std::vector<Force*> forces;
};
