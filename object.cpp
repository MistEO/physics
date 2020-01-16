#include "object.h"

Object::Object(double mass, Coordinate coordinate, Velocity velocity)
    : mass(mass)
    , coordinate(coordinate)
    , velocity(velocity)
{
}

Force Object::sum_of_forces() const
{
    Force sum(0, 0);
    for (const auto& pf : forces) {
        if (pf == nullptr) {
            continue;
        }
        sum = sum + *pf;
    }
    return sum;
}

Coordinate Object::will_go(double time) const
{
    // a = F / m
    Acceleration acceleration = sum_of_forces() / mass;
    // x = Vt + ½at²
    Displacement displacement = velocity * time + 0.5 * acceleration * time * time;

    return coordinate + displacement;
}

void Object::tick(double time)
{
    // a = F / m
    Acceleration acceleration = sum_of_forces() / mass;

    // x = Vt + ½at²
    Displacement displacement = velocity * time + 0.5 * acceleration * time * time;
    coordinate = coordinate + displacement;

    // Vt = V0 + at
    velocity = velocity + acceleration * time;
}