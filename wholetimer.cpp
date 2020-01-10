#include "wholetimer.h"

WholeTimer& WholeTimer::get_instance()
{
    static WholeTimer unique_instance;
    return unique_instance;
}

void WholeTimer::tick()
{
    for (auto& e : object_vector) {
        e->tick();
    }
}

void WholeTimer::push_object(Object* object)
{
    object_vector.push_back(object);
}