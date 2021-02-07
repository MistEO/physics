#include "object.h"

using namespace meophys;

Object::Object(std::string name, long double mass, double elasticity, double friction)
    : _name(std::move(name)),
      _mass(mass),
      _elasticity(elasticity),
      _friction(friction)
{
}

Object::Object(const Object &obj)
    : _name(obj._name),
      _mass(obj._mass),
      _elasticity(obj._elasticity),
      _friction(obj._friction),
      _velocity(obj._velocity)
{
}

Object::Object(Object &&obj)
    : _name(std::move(obj._name)),
      _mass(std::move(obj._mass)),
      _elasticity(std::move(obj._elasticity)),
      _friction(std::move(obj._friction)),
      _velocity(std::move(obj._velocity))
{
}

void Object::exert_force(std::shared_ptr<Force> force_ptr)
{
    std::unique_lock<std::shared_mutex> lock(_force_mutex);
    _forces.emplace_back(std::move(force_ptr));
}

std::shared_ptr<Force> Object::exert_force(Force force)
{
    std::shared_ptr<Force> ptr = std::make_shared<Force>(std::move(force));
    std::unique_lock<std::shared_mutex> lock(_force_mutex);
    _forces.emplace_back(ptr);
    lock.unlock();
    return ptr;
}

Force Object::sum_of_forces() const
{
    std::shared_lock<std::shared_mutex> lock(_force_mutex);
    const auto cur_force = _forces;
    lock.unlock();

    Force sum(0, 0);
    for (auto &&pf : cur_force)
    {
        if (pf == nullptr)
        {
            continue;
        }
        sum += *pf;
    }
    return sum;
}

Object &Object::operator=(const Object &rhs)
{
    _name = rhs._name + "_copy";
    _mass = rhs._mass;
    _elasticity = rhs._elasticity;
    _friction = rhs._friction;
    _velocity = rhs._velocity;
    return *this;
}

Object &Object::operator=(Object &&rhs)
{
    _name = std::move(rhs._name);
    _mass = std::move(rhs._mass);
    _elasticity = std::move(rhs._elasticity);
    _friction = std::move(rhs._friction);
    _velocity = std::move(rhs._velocity);
    return *this;
}