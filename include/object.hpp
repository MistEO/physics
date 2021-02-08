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
        Object()
        {
            std::unique_lock<std::mutex> lock(_id_mutex);
            _unique_id = get_ref_id();
        }
        Object(std::string name, long double mass) : _name(std::move(name)), _mass(mass)
        {
            {
                std::unique_lock<std::mutex> lock(_id_mutex);
                _unique_id = get_ref_id();
            }
            if (mass <= 0)
                throw std::invalid_argument("mass must be positive");
        }
        Object(const Object &rhs) { *this = rhs; }
        Object(Object &&rhs) { *this = std::move(rhs); }

        virtual ~Object() = default;

        virtual const std::string &name() const noexcept { return _name; }
        virtual const long double &mass() const noexcept { return _mass; }
        virtual long double &mass() noexcept { return _mass; }
        virtual const double &radius() const noexcept { return _radius; }
        virtual double &radius() noexcept { return _radius; }
        virtual const double &elasticity() const noexcept { return _elasticity; }
        virtual double &elasticity() noexcept { return _elasticity; }
        virtual const double &friction() const noexcept { return _friction; }
        virtual double &friction() noexcept { return _friction; }
        virtual const int &unique_id() const noexcept { return _unique_id; }

        Object &operator=(const Object &rhs)
        {
            {
                std::unique_lock<std::mutex> lock(_id_mutex);
                _unique_id = get_ref_id();
            }
            _name = rhs._name + "_copy";
            _mass = rhs._mass;
            _radius = rhs._radius;
            _elasticity = rhs._elasticity;
            _friction = rhs._friction;
            return *this;
        }
        Object &operator=(Object &&rhs)
        {
            _unique_id = rhs._unique_id;
            rhs._unique_id = 0;
            _name = std::move(rhs._name);
            _mass = std::move(rhs._mass);
            _radius = std::move(rhs._radius);
            _elasticity = std::move(rhs._elasticity);
            _friction = std::move(rhs._friction);
            return *this;
        }
        const bool operator==(const Object &rhs) const
        {
            return (_unique_id == rhs._unique_id) && (_name == rhs._name);
        }

    protected:
        static const int get_ref_id()
        {
            static int ref_id = 0;
            return ++ref_id;
        }
        std::mutex _id_mutex;
        int _unique_id;           // 唯一id，用于判断相等，计算hash值等
        std::string _name;        // 名字，无实际作用
        long double _mass = 1.0;  // 质量，单位kg
        double _radius = 1.0;     // 半径，单位m
        double _elasticity = 1.0; // 弹性系数
        double _friction = 0;     // 摩擦系数
    };

} // namespace meophys

namespace std
{
    template <>
    class hash<meophys::Object> : public __hash_base<size_t, meophys::Object>
    {
    public:
        size_t operator()(const meophys::Object &rhs) const noexcept
        {
            return ((hash<int>()(rhs.unique_id())) && hash<std::string>()(rhs.name()) << 1);
        }
    };
} // namespace std