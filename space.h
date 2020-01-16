#pragma once

#include <vector>

#include "define.h"
#include "object.h"

class Time;

class Space {
    friend class Time;

public:
    Space() = default;
    ~Space();
    void push_object(Object* object);
    void set_boundary(Coordinate bottomleft, Coordinate topright);
    void set_boundary(std::pair<Coordinate, Coordinate> boundary);
    std::pair<Coordinate, Coordinate> get_boundary() const;
    void set_gravity(double gravity);

private:
    void tick(double time = 1.0);

    std::vector<Object*> objects;
    std::pair<Coordinate, Coordinate> boundary;
    double gravity = 0;
    std::vector<Force*> gravity_vector;
};