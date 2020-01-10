#include "object.h"

Object::Object(double mass, double x_coordinate, double y_coordinate, double x_velocity, double y_velocity)
    : m_mass(mass)
    , m_coordinate(x_coordinate, y_coordinate)
    , m_velocity(x_velocity, y_velocity)
{
}

Velocity Object::velocity() const
{
    return m_velocity;
}

Coordinate Object::coordinate() const
{
    return m_coordinate;
}

Acceleration Object::acceleration() const
{
    return m_acceleration;
}

void Object::tick(int time)
{
    Force force(0, 0);
    for (const auto& f : m_forces) {
        force = force + f;
    }
    // a = F / m
    m_acceleration = force / m_mass;

    // x = Vt + ½at²
    Displacement displacement = m_velocity * time + 0.5 * m_acceleration * time * time;
    m_coordinate = m_coordinate + displacement;

    // Vt = V0 + at
    m_velocity = m_velocity + m_acceleration * time;
}

std::pair<double, double> operator+(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs)
{
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}
std::pair<double, double> operator-(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs)
{
    return std::make_pair(lhs.first - rhs.first, lhs.second - rhs.second);
}
std::pair<double, double> operator*(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs)
{
    return std::make_pair(lhs.first * rhs.first, lhs.second * rhs.second);
}
std::pair<double, double> operator/(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs)
{
    return std::make_pair(lhs.first / rhs.first, lhs.second / rhs.second);
}

std::pair<double, double> operator*(double lhs, const std::pair<double, double>& rhs)
{
    return std::make_pair(lhs * rhs.first, lhs * rhs.second);
}
std::pair<double, double> operator/(double lhs, const std::pair<double, double>& rhs)
{
    return std::make_pair(lhs / rhs.first, lhs / rhs.second);
}

std::pair<double, double> operator*(const std::pair<double, double>& lhs, double rhs)
{
    return std::make_pair(lhs.first * rhs, lhs.second * rhs);
}
std::pair<double, double> operator/(const std::pair<double, double>& lhs, double rhs)
{
    return std::make_pair(lhs.first / rhs, lhs.second / rhs);
}