#include "define.h"

using namespace meophys;

TwoDimensionalValue operator+(const TwoDimensionalValue &lhs, const TwoDimensionalValue &rhs)
{
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}
TwoDimensionalValue operator-(const TwoDimensionalValue &lhs, const TwoDimensionalValue &rhs)
{
    return std::make_pair(lhs.first - rhs.first, lhs.second - rhs.second);
}
TwoDimensionalValue operator*(const TwoDimensionalValue &lhs, const TwoDimensionalValue &rhs)
{
    return std::make_pair(lhs.first * rhs.first, lhs.second * rhs.second);
}
TwoDimensionalValue operator/(const TwoDimensionalValue &lhs, const TwoDimensionalValue &rhs)
{
    return std::make_pair(lhs.first / rhs.first, lhs.second / rhs.second);
}

TwoDimensionalValue operator*(double lhs, const TwoDimensionalValue &rhs)
{
    return std::make_pair(lhs * rhs.first, lhs * rhs.second);
}
TwoDimensionalValue operator/(double lhs, const TwoDimensionalValue &rhs)
{
    return std::make_pair(lhs / rhs.first, lhs / rhs.second);
}

TwoDimensionalValue operator*(const TwoDimensionalValue &lhs, double rhs)
{
    return std::make_pair(lhs.first * rhs, lhs.second * rhs);
}
TwoDimensionalValue operator/(const TwoDimensionalValue &lhs, double rhs)
{
    return std::make_pair(lhs.first / rhs, lhs.second / rhs);
}