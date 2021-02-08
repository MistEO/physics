#pragma once

#include <memory>
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

        virtual std::shared_ptr<Object>
        emplace_object(Object object, ObjectStatus status)
        {
            auto ptr = std::make_shared<Object>(std::move(object));
            std::unique_lock<std::shared_mutex> lock(_objs_mutex);
            _objects.emplace(ptr, std::move(status));
            lock.unlock();
            return ptr;
        }
        virtual std::shared_ptr<Object>
        emplace_object(Object object, Coordinate coordinate, Velocity velocity = Velocity(0, 0), std::vector<Force> forces = std::vector<Force>())
        {
            auto ptr = std::make_shared<Object>(std::move(object));
            std::unique_lock<std::shared_mutex> lock(_objs_mutex);
            _objects.emplace(ptr, ObjectStatus(coordinate, velocity, forces));
            lock.unlock();
            return ptr;
        }

        virtual const ObjectStatus &object_status(std::shared_ptr<Object> objptr) const
        {
            std::shared_lock<std::shared_mutex> lock(_objs_mutex);
            return _objects.at(objptr);
        }

        virtual Time &time() { return *_time_ptr; }

    protected:
        virtual void on_tick(double ticked_time) = 0;

        std::unique_ptr<Time> _time_ptr = nullptr;
        std::unordered_map<std::shared_ptr<Object>, ObjectStatus> _objects;
        mutable std::shared_mutex _objs_mutex;

    private:
        static void callback_tick(Space *p_this, double ticked_time)
        {
            p_this->on_tick(ticked_time);
        }
    };
} // namespace meophys