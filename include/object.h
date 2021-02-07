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
        Object(std::string name, long double mass, double elasticity = 1.0, double friction = 0);
        Object(const Object &obj);
        Object(Object &&obj);

        virtual ~Object() = default;

        virtual void exert_force(std::shared_ptr<Force> force_ptr);
        virtual std::shared_ptr<Force> exert_force(Force force);
        virtual Force sum_of_forces() const;

        virtual const std::string &name() const noexcept { return _name; }
        virtual long double &mass() noexcept { return _mass; }
        virtual double &elasticity() noexcept { return _elasticity; }
        virtual double &friction() noexcept { return _friction; }

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

        Object &operator=(const Object &rhs);
        Object &operator=(Object &&rhs);

    protected:
        std::string _name;        // 名字，无实际作用
        long double _mass = 1.0;  // 质量
        double _elasticity = 1.0; // 弹性系数
        double _friction = 0;     // 摩擦系数
        Velocity _velocity;       // 速率
        mutable std::shared_mutex _velocity_mutex;
        std::vector<std::shared_ptr<Force>> _forces; // 所受的力
        mutable std::shared_mutex _force_mutex;

        // TODO: 物体面积
    };
} // namespace meophys