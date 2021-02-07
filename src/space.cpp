#include "space.h"

#include <cmath>
#include "object.h"

using namespace meophys;

std::shared_ptr<Object> Space::emplace_object(Object object, Coordinate coor)
{
    std::shared_ptr<Object> ptr = std::make_shared<Object>(std::move(object));
    std::unique_lock<std::shared_mutex> lock(_objs_mutex);
    _objects.emplace(ptr, std::move(coor));
    lock.unlock();
    return ptr;
}

void Space::on_tick(double ticked_time)
{
    std::shared_lock<std::shared_mutex> rdlock(_objs_mutex);
    auto temp_objects = _objects;
    rdlock.unlock();

    for (auto &&[obj, coor] : temp_objects)
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
    _objects = std::move(temp_objects);
}