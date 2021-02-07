#include "interstellar.h"

#include <cmath>
#include "object.h"

using namespace meophys;

void Interstellar::on_tick(double ticked_time)
{
    std::shared_lock<std::shared_mutex> rdlock(_objs_mutex);
    auto temp_objects = _objects;
    const auto cur_objects = _objects;
    rdlock.unlock();

    for (auto &&[obj, coor] : temp_objects)
    {
        if (obj == nullptr)
        {
            continue;
        }

        Force gravitation(0, 0);
        auto &&[c_x, c_y] = coor;
        for (auto &&[cur_obj, cur_coor] : cur_objects)
        {
            if (obj == cur_obj)
            {
                continue;
            }
            auto &&[cur_c_x, cur_c_y] = cur_coor;

            // The universal gravitation, F = GMm/r²
            double r2 = std::pow(c_x - cur_c_x, 2) + std::pow(c_y - cur_c_y, 2);
            double f_value = GravitationConstant * obj->mass() * cur_obj->mass() / r2;

            double r = std::sqrt(r2);
            double f_x = (cur_c_x - c_x) * f_value / r;
            double f_y = (cur_c_y - c_y) * f_value / r;
            gravitation += Force(f_x, f_y);
        }

        // a = F / m
        Acceleration acc = (obj->sum_of_forces() + gravitation) / obj->mass();

        // x = Vt + ½at²
        Displacement d = obj->get_velocity() * ticked_time + 0.5 * acc * std::pow(ticked_time, 2);
        coor += d;

        // Vt = V0 + at
        obj->set_velocity(obj->get_velocity() + acc * ticked_time);
    }
    std::unique_lock<std::shared_mutex> wrlock(_objs_mutex);
    _objects = std::move(temp_objects);
}