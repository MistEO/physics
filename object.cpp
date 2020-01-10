#include "object.h"

Object::Object(double mass, double x_coordinate, double y_coordinate, double x_velocity, double y_velocity)
    : mass(mass)
    , x_coordinate(x_coordinate)
    , y_coordinate(y_coordinate)
    , x_velocity(x_velocity)
    , y_velocity(y_velocity)
{
}

std::pair<double, double> Object::velocity() const
{
    return std::make_pair(x_velocity, y_velocity);
}

std::pair<double, double> Object::coordinate() const
{
    return std::make_pair(x_coordinate, y_coordinate);
}

std::pair<double, double> Object::acceleration() const
{
    return std::make_pair(x_acceleration, y_acceleration);
}

void Object::tick(int time)
{
    Force force(0, 0);
    for (const auto& f : forces) {
        force.first += f.first;
        force.second += f.second;
    }
    // a = F / m
    x_acceleration = force.first / mass;
    y_acceleration = force.second / mass;

    // x = Vt + ½at²
    double x = x_velocity * time + 0.5 * x_acceleration * time * time;
    double y = y_velocity * time + 0.5 * y_acceleration * time * time;

    x_coordinate += x;
    y_coordinate += y;

    // Vt = V0 + at
    x_velocity += x_acceleration * time;
    y_velocity += y_acceleration * time;
}