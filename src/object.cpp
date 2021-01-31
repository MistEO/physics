#include "object.h"

using namespace meophys;

void Object::exert_force(std::shared_ptr<Force> force)
{
    _forces.emplace_back(std::move(force));
}

Force Object::sum_of_forces() const
{
    Force sum(0, 0);
    for (auto &&pf : _forces)
    {
        if (pf == nullptr)
        {
            continue;
        }
        sum = sum + *pf;
    }
    return sum;
}