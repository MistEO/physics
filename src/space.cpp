#include "space.h"

#include <cmath>
#include "object.h"

using namespace meophys;

Space::Space()
    : _time_ptr(std::make_unique<Time>(callback_this, this))
{
}

void Space::push_object(std::shared_ptr<Object> object, Coordinate coor)
{
    _objects.emplace_back(std::make_pair(std::move(object), std::move(coor)));
}

void Space::callback_this(Space *p_this)
{
    p_this->on_tick();
}

void Space::on_tick()
{
    for (auto &&[obj, coor] : _objects)
    {
        if (obj == nullptr)
        {
            return;
        }

        // a = F / m
        Acceleration acc = obj->sum_of_forces() / obj->mass();

        // x = Vt + ½at²
        Displacement d = obj->velocity() * PlankTime + 0.5 * acc * std::pow(PlankTime, 2);
        coor += d;

        // Vt = V0 + at
        obj->velocity() += acc * PlankTime;
    }
}