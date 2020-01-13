#pragma once

#include <utility>
#include <vector>

#include "define.h"

class Space;

class Object {
public:
    Object(double mass, Coordinate coordinate, Velocity velocity = Velocity(0, 0));

    void push_force(const Force& f);

    double& mass();
    double& elasticity();
    Coordinate& coordinate();
    Velocity& velocity();
    Acceleration& acceleration();
    Force sum_of_forces() const;

private:
    double m_mass;
    Coordinate m_coordinate;
    Velocity m_velocity;
    Acceleration m_acceleration;
    std::vector<Force> m_forces;
    double m_elasticity = 1.0;
};
