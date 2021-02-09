#pragma once

#include <cmath>
#include <utility>
#include <type_traits>

namespace meophys
{
    static constexpr double PlanckLength = 1e-16;
    static constexpr double FloatDiff = 1e-20;

    // TODO: 三维量

    using TDValue = std::pair<double, double>; // 二维量

    using Force = TDValue;        // 力
    using Velocity = TDValue;     // 速率
    using Acceleration = TDValue; // 加速度
    using Displacement = TDValue; // 位移

    using Coordinate = TDValue; // 坐标

    template <typename T>
    const T distance_squared(const std::pair<T, T> &lhs, const std::pair<T, T> &rhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        return std::pow(lhs.first - rhs.first, 2) + std::pow(lhs.second - rhs.second, 2);
    }

    template <typename T>
    const T distance(const std::pair<T, T> &lhs, const std::pair<T, T> &rhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        return std::sqrt(distance_squared(lhs, rhs));
    }

    template <typename T>
    const T abs(const std::pair<T, T> &lhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        return std::sqrt(std::pow(lhs.first, 2) + std::pow(lhs.second, 2));
    }

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
    std::pair<T, T> &operator+=(std::pair<T, T> &lhs, const std::pair<T, T> &rhs) noexcept
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        lhs.first += rhs.first;
        lhs.second += rhs.second;
        return lhs;
    }
    template <typename T>
    std::pair<T, T> operator-(const std::pair<T, T> &rhs) noexcept
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        rhs.first = -rhs.first;
        rhs.second = -rhs.second;
        return rhs;
    }

} // namespace meophys