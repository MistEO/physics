#pragma once

#include <vector>
#include <string>
#include <stdexcept>
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
        Object(const Object &obj) = default;
        Object(Object &&obj) = default;

        virtual ~Object() = default;

        virtual const std::string &name() const noexcept { return _name; }
        virtual long double &mass() noexcept { return _mass; }
        virtual double &radius() noexcept { return _radius; }
        virtual double &elasticity() noexcept { return _elasticity; }
        virtual double &friction() noexcept { return _friction; }

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