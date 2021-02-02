#pragma once

#include <vector>
#include <memory>
#include "define.h"

namespace meophys
{
    class Object
    {
    public:
        Object() = default;
        Object(double mass, double elasticity = 1.0, double friction = 0, Velocity initial_speed = Velocity(0, 0))
            : _mass(mass), _elasticity(elasticity), _friction(friction), _velocity(initial_speed) {}
        virtual ~Object() = default;

        virtual void exert_force(std::shared_ptr<Force> force);
        virtual void exert_force(Force force);
        virtual Force sum_of_forces() const;

        virtual double &mass() noexcept { return _mass; }
        virtual double &elasticity() noexcept { return _elasticity; }
        virtual double &friction() noexcept { return _friction; }
        virtual Velocity &velocity() noexcept { return _velocity; }

    protected:
        double _mass = 1.0;                          // 质量
        double _elasticity = 1.0;                    // 弹性系数
        double _friction = 0;                        // 摩擦系数
        Velocity _velocity;                          // 速率
        std::vector<std::shared_ptr<Force>> _forces; // 所受的力
    };
} // namespace meophys