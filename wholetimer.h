#pragma once

#include "object.h"
#include <vector>

class WholeTimer {
public:
    WholeTimer& get_instance();
    void push_object(Object* object);

private:
    WholeTimer() = default;

    void tick();

    std::vector<Object*> object_vector;
};