#include "planet.h"

Planet::Planet(double mass, Coordinate coordinate, Velocity velocity)
    : Object(mass, coordinate, velocity)
{
}

Force Planet::sum_of_forces() const
{
    Force sum(0, 0);
    for (const auto& pair : gravity_pair_vector) {
        auto pf = pair.second;
        if (pf == nullptr) {
            continue;
        }
        sum = sum + *pf;
    }
    return sum + Object::sum_of_forces();
}