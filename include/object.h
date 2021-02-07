#pragma once

#include <vector>
#include <memory>
#include <string>
#include <shared_mutex>
#include "define.h"

namespace meophys
{
    class Object
    {
    public:
        Object() = delete;
        Object(std::string name, long double mass, double elasticity = 1.0, double friction = 0, Velocity initial_speed = Velocity(0, 0))
            : _name(std::move(name)), _mass(mass), _elasticity(elasticity), _friction(friction), _velocity(initial_speed) {}
        Object(const Object &obj)
            : _name(obj._name), _mass(obj._mass), _elasticity(obj._elasticity), _friction(obj._friction), _velocity(obj._velocity) {}
        Object(Object &&obj)
            : _name(std::move(obj._name)), _mass(std::move(obj._mass)), _elasticity(std::move(obj._elasticity)), _friction(std::move(obj._friction)), _velocity(std::move(obj._velocity)) {}
        virtual ~Object() = default;

        virtual void exert_force(std::shared_ptr<Force> force_ptr);
        virtual std::shared_ptr<Force> exert_force(Force force);
        virtual Force sum_of_forces() const;

        virtual const std::string &name() const noexcept { return _name; }
        virtual const long double &mass() const noexcept { return _mass; }
        virtual const double &elasticity() const noexcept { return _elasticity; }
        virtual const double &friction() const noexcept { return _friction; }
        virtual const Velocity get_velocity() const
        {
            std::shared_lock<std::shared_mutex> lock(_velocity_mutex);
            return _velocity;
        }
        virtual void set_velocity(Velocity velocity)
        {
            std::unique_lock<std::shared_mutex> lock(_velocity_mutex);
            _velocity = std::move(velocity);
        }

        Object &operator=(const Object &) = delete;
        Object &operator=(Object &&) = delete;

    protected:
        const std::string _name;        // 名字，无实际作用
        const long double _mass = 1.0;  // 质量
        const double _elasticity = 1.0; // 弹性系数
        const double _friction = 0;     // 摩擦系数
        Velocity _velocity;             // 速率
        mutable std::shared_mutex _velocity_mutex;
        std::vector<std::shared_ptr<Force>> _forces; // 所受的力
        mutable std::shared_mutex _force_mutex;

        // TODO: 物体面积
    };
} // namespace meophys