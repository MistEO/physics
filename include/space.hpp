#pragma once

#include <unordered_map>
#include <shared_mutex>

#include "define.h"
#include "phys_time.h"
#include "object.hpp"
#include "object_status.hpp"

namespace meophys
{
    class Space
    {
    public:
        Space()
            : _time_ptr(std::make_unique<Time>(callback_tick, this)) {}
        virtual ~Space() = default;

        virtual std::unordered_map<Object, ObjectStatus>::iterator
        emplace_object(Object object, ObjectStatus status)
        {
            std::unique_lock<std::shared_mutex> lock(_objs_mutex);
            return _objects.emplace(std::move(object), std::move(status)).first;
        }
        virtual std::unordered_map<Object, ObjectStatus>::iterator
        emplace_object(Object object, Coordinate coordinate, Velocity velocity = Velocity(0, 0), std::vector<Force> forces = std::vector<Force>())
        {
            std::unique_lock<std::shared_mutex> lock(_objs_mutex);
            return _objects.emplace(std::move(object), ObjectStatus(coordinate, velocity, forces)).first;
        }

        virtual Time &time() { return *_time_ptr; }

    protected:
        virtual void on_tick(double ticked_time) = 0;

        std::unique_ptr<Time> _time_ptr = nullptr;
        std::unordered_map<Object, ObjectStatus> _objects;
        mutable std::shared_mutex _objs_mutex;

    private:
        static void callback_tick(Space *p_this, double ticked_time)
        {
            p_this->on_tick(ticked_time);
        }
    };
} // namespace meophys