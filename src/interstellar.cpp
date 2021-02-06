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
        auto &&[c_x, c_y] = coor;
        for (auto &&[anohter_obj, anohter_coor] : pre_objects)
        {
            if (obj == anohter_obj)
            {
                continue;
            }
            auto &&[another_c_x, another_c_y] = anohter_coor;

            // The universal gravitation, F = GMm/r²
            double r2 = std::pow(c_x - another_c_x, 2) + std::pow(c_y - another_c_y, 2);
            double f_value = Gravitation * obj->mass() * anohter_obj->mass() / r2;

            double r = std::sqrt(r2);
            double f_x = (another_c_x - c_x) * f_value / r;
            double f_y = (another_c_y - c_y) * f_value / r;
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