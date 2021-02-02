#pragma once

#include <utility>

namespace meophys
{
    static constexpr double PlanckLength = 1e-15;
    static constexpr double PlanckTime = 1e-3; // seconds

    using TwoDimensionalValue = std::pair<double, double>; // 二维量

    using Force = TwoDimensionalValue;        // 力
    using Velocity = TwoDimensionalValue;     // 速率
    using Acceleration = TwoDimensionalValue; // 加速度
    using Displacement = TwoDimensionalValue; // 位移

    using Coordinate = TwoDimensionalValue; // 坐标

    TwoDimensionalValue operator+(const TwoDimensionalValue &lhs, const TwoDimensionalValue &rhs);
    TwoDimensionalValue operator-(const TwoDimensionalValue &lhs, const TwoDimensionalValue &rhs);
    TwoDimensionalValue operator*(const TwoDimensionalValue &lhs, const TwoDimensionalValue &rhs);
    TwoDimensionalValue operator/(const TwoDimensionalValue &lhs, const TwoDimensionalValue &rhs);

    TwoDimensionalValue operator*(double lhs, const TwoDimensionalValue &rhs);
    TwoDimensionalValue operator/(double lhs, const TwoDimensionalValue &rhs);

    TwoDimensionalValue operator*(const TwoDimensionalValue &lhs, double rhs);
    TwoDimensionalValue operator/(const TwoDimensionalValue &lhs, double rhs);

    TwoDimensionalValue &operator+=(TwoDimensionalValue &lhs, const TwoDimensionalValue &rhs);

} // namespace meophys