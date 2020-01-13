#pragma once

#include <vector>

#include "define.h"
#include "object.h"

class Space {
public:
    void push_object(Object* object);
    void push_boundary(const Line& line);

    void tick(double time = 1.0);

private:
    std::vector<Line> boundaries;
    std::vector<Object*> objects;
};