#include "space.h"

#include <cmath>
#include <iostream>

Space::~Space()
{
    for (auto& g : gravity_vector) {
        if (g != nullptr) {
            delete g;
            g = nullptr;
        }
    }
}

void Space::push_object(Object* object)
{
    if (object == nullptr) {
        std::cerr << "the object is empty" << std::endl;
        return;
    }
    for (const auto& pre_obj : objects) {
    }
    objects.push_back(object);
}

void Space::tick(double time)
{
    for (const auto& object : objects) {
        if (object == nullptr) {
            std::cerr << "the object is null" << std::endl;
            continue;
        }
        tick_object(object, time);
    }
}

void Space::tick_object(Object* object, double time)
{
    if (object == nullptr) {
        std::cerr << "the object is null" << std::endl;
        return;
    }
    object->tick(time);
}