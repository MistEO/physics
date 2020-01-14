#pragma once

#include <vector>

#include "define.h"
#include "object.h"

class Time;

class Space {
    friend class Time;

public:
    void push_object(Object* object);
    void push_boundary(const Boundary& boundary);

private:
    void tick(double time = 1.0);

    std::vector<Boundary> boundaries;
    std::vector<Object*> objects;
};