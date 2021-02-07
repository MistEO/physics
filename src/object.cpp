#include "object.h"

using namespace meophys;

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