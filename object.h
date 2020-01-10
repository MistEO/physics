#pragma once

#include <utility>
#include <vector>

class WholeTimer;

typedef std::pair<double, double> Force;
typedef std::pair<double, double> Coordinate;
typedef std::pair<double, double> Velocity;
typedef std::pair<double, double> Acceleration;
typedef std::pair<double, double> Displacement;

class Object {
    friend class WholeTimer;

public:
    Object(double mass, double x_coordinate, double y_coordinate, double x_velocity = 0, double y_velocity = 0);

    Coordinate coordinate() const;
    Velocity velocity() const;
    Acceleration acceleration() const;

    void tick(int time = 1);

private:
    const double m_mass;
    Coordinate m_coordinate;
    Velocity m_velocity;
    Acceleration m_acceleration;
    std::vector<Force> m_forces;
};

std::pair<double, double> operator+(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs);
std::pair<double, double> operator-(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs);
std::pair<double, double> operator*(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs);
std::pair<double, double> operator/(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs);

std::pair<double, double> operator*(double lhs, const std::pair<double, double>& rhs);
std::pair<double, double> operator/(double lhs, const std::pair<double, double>& rhs);

std::pair<double, double> operator*(const std::pair<double, double>& lhs, double rhs);
std::pair<double, double> operator/(const std::pair<double, double>& lhs, double rhs);