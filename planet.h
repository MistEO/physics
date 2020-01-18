#pragma once

#include "object.h"

class Planet : public Object {
public:
    Planet(double mass, Coordinate coordinate = Coordinate(0, 0), Velocity velocity = Velocity(0, 0));
};