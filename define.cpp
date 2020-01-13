#include "define.h"

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