#include "object.h"

Object::Object(double mass, Coordinate coordinate, Velocity velocity)
    : m_mass(mass)
    , m_coordinate(coordinate)
    , m_velocity(velocity)
{
}

void Object::push_force(const Force& f)
{
    m_forces.push_back(f);
}

double& Object::mass()
{
    return m_mass;
}

double& Object::elasticity()
{
    return m_elasticity;
}

Velocity& Object::velocity()
{
    return m_velocity;
}

Coordinate& Object::coordinate()
{
    return m_coordinate;
}

Acceleration& Object::acceleration()
{
    return m_acceleration;
}

Force Object::sum_of_forces() const
{
    Force sum(0, 0);
    for (const auto& f : m_forces) {
        sum = sum + f;
    }
    return sum;
}