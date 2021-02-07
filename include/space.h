#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <shared_mutex>

#include "define.h"
#include "phys_time.h"

namespace meophys
{
    class Object;

    class Space
    {
    public:
        Space()
            : _time_ptr(std::make_unique<Time>(callback_tick, this)) {}
        virtual ~Space() = default;

        virtual void emplace_object(std::shared_ptr<Object> obj_ptr, Coordinate coor)
        {
            std::unique_lock<std::shared_mutex> lock(_objs_mutex);
            _objects.emplace(std::move(obj_ptr), std::move(coor));
        }

        virtual std::shared_ptr<Object> emplace_object(Object object, Coordinate coor);

        virtual const Coordinate get_coor(std::shared_ptr<Object> object) const
        {
            std::shared_lock<std::shared_mutex> lock(_objs_mutex);
            return _objects.at(object);
        }

        virtual void set_coor(std::shared_ptr<Object> object, Coordinate coor)
        {
            std::unique_lock<std::shared_mutex> lock(_objs_mutex);
            _objects[object] = std::move(coor);
        }

        virtual Time &time() { return *_time_ptr; }

    protected:
        virtual void on_tick(double ticked_time);

        std::unique_ptr<Time> _time_ptr = nullptr;
        std::unordered_map<std::shared_ptr<Object>, Acceleration> _objects;
        mutable std::shared_mutex _objs_mutex;

    private:
        static void callback_tick(Space *p_this, double ticked_time)
        {
            p_this->on_tick(ticked_time);
        }
    };
} // namespace meophys