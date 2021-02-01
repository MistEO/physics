#include "world.h"

using namespace meophys;

World::World()
    : Space()
{
    // _time_ptr = (std::make_unique<Time>(world_callback_when_ticktime, this));
}

void World::callback_when_ticktime()
{
    for (auto &&[obj, coor] : _objects)
    {
        // // a = F / m
        // Acceleration acc = obj->sum_of_forces() / obj->mass();

        // // x = Vt + ½at²
        // Displacement d = obj->velocity() * PlankTime + 0.5 * acc * std::pow(PlankTime, 2);
        // coor = coor + d;

        // // Vt = V0 + at
        // obj->velocity() = obj->velocity() + acc * PlankTime;
    }
}