#pragma once

#include <utility>
#include <type_traits>

namespace meophys
{
    static constexpr double PlanckLength = 1e-15;

    using TDValue = std::pair<double, double>; // 二维量

    using Force = TDValue;        // 力
    using Velocity = TDValue;     // 速率
    using Acceleration = TDValue; // 加速度
    using Displacement = TDValue; // 位移

    using Coordinate = TDValue; // 坐标

    template <typename T>
    std::pair<T, T> operator+(const std::pair<T, T> &lhs, const std::pair<T, T> &rhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
    }

    template <typename T>
    std::pair<T, T> operator-(const std::pair<T, T> &lhs, const std::pair<T, T> &rhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        return std::make_pair(lhs.first - rhs.first, lhs.second - rhs.second);
    }

    template <typename T>
    std::pair<T, T> operator*(const std::pair<T, T> &lhs, const std::pair<T, T> &rhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        return std::make_pair(lhs.first * rhs.first, lhs.second * rhs.second);
    }

    template <typename T>
    std::pair<T, T> operator/(const std::pair<T, T> &lhs, const std::pair<T, T> &rhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        return std::make_pair(lhs.first / rhs.first, lhs.second / rhs.second);
    }

    template <typename T>
    std::pair<T, T> operator*(double lhs, const std::pair<T, T> &rhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        return std::make_pair(lhs * rhs.first, lhs * rhs.second);
    }

    template <typename T>
    std::pair<T, T> operator/(double lhs, const std::pair<T, T> &rhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        return std::make_pair(lhs / rhs.first, lhs / rhs.second);
    }

    template <typename T>
    std::pair<T, T> operator*(const std::pair<T, T> &lhs, double rhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        return std::make_pair(lhs.first * rhs, lhs.second * rhs);
    }

    template <typename T>
    std::pair<T, T> operator/(const std::pair<T, T> &lhs, double rhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        return std::make_pair(lhs.first / rhs, lhs.second / rhs);
    }

    template <typename T>
    std::pair<T, T> &operator+=(std::pair<T, T> &lhs, const std::pair<T, T> &rhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        return lhs = lhs + rhs;
    }

} // namespace meophys