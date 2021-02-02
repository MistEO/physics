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
        virtual Time &time() { return *_time_ptr; }

    protected:
        virtual void on_tick();

        std::unique_ptr<Time> _time_ptr = nullptr;
        std::vector<std::pair<std::shared_ptr<Object>, Acceleration>> _objects;

    private:
        static void callback_this(Space *p_this);
    };
} // namespace meophys