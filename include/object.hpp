#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <atomic>
#include <mutex>
#include "define.h"

namespace meophys
{
    class Object
    {
    public:
        Object() = default;
        Object(std::string name, long double mass) : _name(std::move(name)), _mass(mass)
        {
            if (mass <= 0)
                throw std::invalid_argument("mass must be positive");
        }
        Object(const Object &rhs) = default;
        Object(Object &&rhs) = default;

        ~Object() = default;

        const std::string &name() const noexcept { return _name; }
        const long double &mass() const noexcept { return _mass; }
        long double &mass() noexcept { return _mass; }
        const double &radius() const noexcept { return _radius; }
        double &radius() noexcept { return _radius; }
        const double &elasticity() const noexcept { return _elasticity; }
        double &elasticity() noexcept { return _elasticity; }
        const double &friction() const noexcept { return _friction; }
        double &friction() noexcept { return _friction; }

        Object &operator=(const Object &rhs) = default;
        Object &operator=(Object &&rhs) = default;

    protected:
        std::string _name;        // 名字，无实际作用
        long double _mass = 1.0;  // 质量，单位kg
        double _radius = 1.0;     // 半径，单位m
        double _elasticity = 1.0; // 弹性系数
        double _friction = 0;     // 摩擦系数
    };

} // namespace meophys