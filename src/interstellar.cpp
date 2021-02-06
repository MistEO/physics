#include "interstellar.h"

#include <cmath>
#include "object.h"

using namespace meophys;

void Interstellar::on_tick(double ticked_time)
{
    std::shared_lock<std::shared_mutex> rdlock(_objs_mutex);
    auto temp = _objects;
    const auto pre_objects = _objects;
    rdlock.unlock();

    for (auto &&[obj, coor] : temp)
    {
        if (obj == nullptr)
        {
            continue;
        }

        Force gravitation(0, 0);
        for (auto &&[pre_obj, pre_coor] : pre_objects)
        {
            if (obj == pre_obj)
            {
                continue;
            }
            if (coor == pre_coor)
            {
                // 相撞的情况，TODO
            }
            // The universal gravitation, F = GMm/r²
            double r2 = std::pow(coor.first - pre_coor.first, 2) + std::pow(coor.second - pre_coor.second, 2);
            double f_value = Gravitation * obj->mass() * pre_obj->mass() / r2;

            double r = std::sqrt(r2);
            double f_x = (pre_coor.first - coor.first) * f_value / r;
            double f_y = (pre_coor.second - coor.second) * f_value / r;
            gravitation += Force(f_x, f_y);
        }

        // a = F / m
        Acceleration acc = (obj->sum_of_forces() + gravitation) / obj->mass();

        // x = Vt + ½at²
        Displacement d = obj->velocity() * ticked_time + 0.5 * acc * std::pow(ticked_time, 2);
        coor += d;

        // Vt = V0 + at
        obj->velocity() += acc * ticked_time;
    }
    std::unique_lock<std::shared_mutex> wrlock(_objs_mutex);
    _objects = std::move(temp);
}