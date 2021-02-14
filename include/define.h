#pragma once

#include <cmath>
#include <utility>
#include <type_traits>

namespace meophys
{
    static constexpr double PlanckLength = 1e-8;
    static constexpr double FloatDiff = 1e-20;

    // TODO: 三维量

    using Vector = std::pair<double, double>; // 二维量

    using Force = Vector;        // 力
    using Velocity = Vector;     // 速率
    using Acceleration = Vector; // 加速度
    using Displacement = Vector; // 位移

    using Coordinate = Vector; // 坐标

    template <typename T>
    T distance_squared(const std::pair<T, T> &lhs, const std::pair<T, T> &rhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        return std::pow(lhs.first - rhs.first, 2) + std::pow(lhs.second - rhs.second, 2);
    }

    template <typename T>
    T distance(const std::pair<T, T> &lhs, const std::pair<T, T> &rhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        return std::sqrt(distance_squared(lhs, rhs));
    }

    template <typename T>
    T norm(const std::pair<T, T> &lhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");
        return std::sqrt(std::pow(lhs.first, 2) + std::pow(lhs.second, 2));
    }

    template <typename T>
    std::pair<T, T> abs(const std::pair<T, T> &lhs)
    {
        static_assert(std::is_arithmetic<T>::value, "Parameter is not arithmetic.");

        if (std::is_integral<T>::value)
        {
            return std::make_pair(std::abs(lhs.first), std::abs(lhs.second));
        }
        else if (std::is_floating_point<T>::value)
        {
            return std::make_pair(std::fabs(lhs.first), std::fabs(lhs.second));
        }
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
        auto temp = rhs;
        temp.first = -rhs.first;
        temp.second = -rhs.second;
        return temp;
    }

} // namespace meophys