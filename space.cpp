#include "space.h"

#include <cmath>

void Space::push_object(Object* object)
{
    objects.push_back(object);
}

void Space::push_boundary(const Line& boundary)
{
    boundaries.push_back(boundary);
}

void Space::tick(double time)
{
    for (const auto& object : objects) {
        // a = F / m
        object->acceleration() = object->sum_of_forces() / object->mass();

        // x = Vt + ½at²
        Displacement displacement = object->velocity() * time + 0.5 * object->acceleration() * time * time;
        // 计算是否超出边界
        Coordinate temp = object->coordinate() + displacement;
        bool y_out = false, x_out = false;
        for (const auto& boundary : boundaries) {
            y_out |= (boundary.first * object->coordinate().first + boundary.second - object->coordinate().second)
                    * (boundary.first * temp.first + boundary.second - temp.second)
                <= 0;
            x_out |= false;
        }
        if (y_out) {
            temp.second = -temp.second;
        }
        object->coordinate() = temp;

        // Vt = V0 + at
        object->velocity() = object->velocity() + object->acceleration() * time;
        if (y_out) {
            object->velocity().second = -object->velocity().second * std::pow(object->elasticity(), 2);
        }
    }
}