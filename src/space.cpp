#include "space.h"

#include <cmath>
#include "object.h"

using namespace meophys;

Space::Space()
    : _time_ptr(std::make_unique<Time>(callback_tick, this))
{
}

void Space::emplace_object(std::shared_ptr<Object> object, Coordinate coor)
{
    std::unique_lock<std::shared_mutex> lock(_objs_mutex);
    _objects.emplace(std::move(object), std::move(coor));
}

void Space::callback_tick(Space *p_this, double ticked_time)
{
    p_this->on_tick(ticked_time);
}

void Space::on_tick(double ticked_time)
{
    std::shared_lock<std::shared_mutex> rdlock(_objs_mutex);
    auto temp = _objects;
    rdlock.unlock();

    for (auto &&[obj, coor] : temp)
    {
        if (obj == nullptr)
        {
            continue;
        }

        // a = F / m
        Acceleration acc = obj->sum_of_forces() / obj->mass();

        // x = Vt + ½at²
        Displacement d = obj->velocity() * ticked_time + 0.5 * acc * std::pow(ticked_time, 2);
        coor += d;

        // Vt = V0 + at
        obj->velocity() += acc * ticked_time;
    }
    std::unique_lock<std::shared_mutex> wrlock(_objs_mutex);
    _objects = std::move(temp);
}