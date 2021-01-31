#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "define.h"
#include "phys_time.h"

namespace meophys
{
    class Object;

    class Space
    {
    public:
        Space();
        virtual ~Space() = default;

        virtual void push_object(std::shared_ptr<Object> object, Coordinate coor);
        virtual Time &time() { return _time; }

    protected:
        Time _time;
        std::function<void(Space *)> _callback_when_ticktime = space_callback_when_ticktime;
        std::vector<std::pair<std::shared_ptr<Object>, Acceleration>> _objects;

    private:
        static void space_callback_when_ticktime(Space *p_this);
    };
} // namespace meophys