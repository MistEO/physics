#include "space.h"
#include <cmath>
#include "object.h"

using namespace meophys;

Space::Space()
    : _time(_callback_when_ticktime, this)
{
}

void Space::push_object(std::shared_ptr<Object> object, Coordinate coor)
{
    _objects.emplace_back(std::make_pair(std::move(object), std::move(coor)));
}

Time &Space::time()
{
    return _time;
}

void Space::space_callback_when_ticktime(Space *p_this)
{
    for (auto &&[obj, coor] : p_this->_objects)
    {
        // a = F / m
        Acceleration acc = obj->sum_of_forces() / obj->mass();

        // x = Vt + ½at²
        Displacement d = obj->velocity() * PlankTime + 0.5 * acc * std::pow(PlankTime, 2);
        coor = coor + d;

        // Vt = V0 + at
        obj->velocity() = obj->velocity() + acc * PlankTime;
    }
}