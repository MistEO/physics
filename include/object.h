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
        virtual ~Object() = default;

        virtual void exert_force(std::shared_ptr<Force> force);
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