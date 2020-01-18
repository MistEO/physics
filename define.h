#pragma once

#include <utility>

constexpr double DOUBLE_DIFF = 1e-15;

typedef std::pair<double, double> Force; // 力
typedef std::pair<double, double> Coordinate; // 坐标
typedef std::pair<double, double> Velocity; // 速率
typedef std::pair<double, double> Acceleration; // 加速度
typedef std::pair<double, double> Displacement; // 位移

std::pair<double, double> operator+(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs);
std::pair<double, double> operator-(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs);
std::pair<double, double> operator*(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs);
std::pair<double, double> operator/(const std::pair<double, double>& lhs, const std::pair<double, double>& rhs);

std::pair<double, double> operator*(double lhs, const std::pair<double, double>& rhs);
std::pair<double, double> operator/(double lhs, const std::pair<double, double>& rhs);

std::pair<double, double> operator*(const std::pair<double, double>& lhs, double rhs);
std::pair<double, double> operator/(const std::pair<double, double>& lhs, double rhs);