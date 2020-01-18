#include "space.h"

#include <cmath>
#include <iostream>

void Space::push_object(Object* object)
{
    if (object == nullptr) {
        std::cerr << "the object is empty" << std::endl;
        return;
    }
    objects.push_back(object);
}

void Space::tick(double time)
{
    for (const auto& object : objects) {
        tick_object(object, time);
    }
}

void Space::tick_object(Object* object, double time)
{
}