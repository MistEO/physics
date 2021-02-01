#include "define.h"

using namespace meophys;

TwoDimensionalValue meophys::operator+(const TwoDimensionalValue &lhs, const TwoDimensionalValue &rhs)
{
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}
TwoDimensionalValue meophys::operator-(const TwoDimensionalValue &lhs, const TwoDimensionalValue &rhs)
{
    return std::make_pair(lhs.first - rhs.first, lhs.second - rhs.second);
}
TwoDimensionalValue meophys::operator*(const TwoDimensionalValue &lhs, const TwoDimensionalValue &rhs)
{
    return std::make_pair(lhs.first * rhs.first, lhs.second * rhs.second);
}
TwoDimensionalValue meophys::operator/(const TwoDimensionalValue &lhs, const TwoDimensionalValue &rhs)
{
    return std::make_pair(lhs.first / rhs.first, lhs.second / rhs.second);
}

TwoDimensionalValue meophys::operator*(double lhs, const TwoDimensionalValue &rhs)
{
    return std::make_pair(lhs * rhs.first, lhs * rhs.second);
}
TwoDimensionalValue meophys::operator/(double lhs, const TwoDimensionalValue &rhs)
{
    return std::make_pair(lhs / rhs.first, lhs / rhs.second);
}

TwoDimensionalValue meophys::operator*(const TwoDimensionalValue &lhs, double rhs)
{
    return std::make_pair(lhs.first * rhs, lhs.second * rhs);
}
TwoDimensionalValue meophys::operator/(const TwoDimensionalValue &lhs, double rhs)
{
    return std::make_pair(lhs.first / rhs, lhs.second / rhs);
}