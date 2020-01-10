#pragma once

#include <utility>
#include <vector>

class WholeTimer;

typedef std::pair<double, double> Force; // 力
typedef std::pair<double, double> Coordinate; // 坐标
typedef std::pair<double, double> Velocity; // 速率
typedef std::pair<double, double> Acceleration; // 加速度
typedef std::pair<double, double> Displacement; // 位移

class Object {
    friend class WholeTimer;

public:
    Object(double mass, double x_coordinate, double y_coordinate, double x_velocity = 0, double y_velocity = 0);

    void push_force(const Force& f);

    Coordinate coordinate() const;
    Velocity velocity() const;
    Acceleration acceleration() const;
    Force sum_of_force() const;

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