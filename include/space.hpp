#pragma once

#include <memory>
#include <unordered_map>
#include <shared_mutex>
#include <queue>
#include <tuple>

#include "define.h"
#include "phys_time.h"
#include "object.hpp"
#include "object_status.hpp"

namespace meophys
{
    class Space
    {
        enum class QueueBehavior
        {
            Emplace,
            Erase
        };

    public:
        Space() : _time_ptr(std::make_unique<Time>(callback_tick, this))
        {
        }
        virtual ~Space() = default;

        virtual std::shared_ptr<Object>
        emplace_object(Object object, ObjectStatus status)
        {
            auto ptr = std::make_shared<Object>(std::move(object));
            _task_queue.emplace(QueueBehavior::Emplace, ptr, std::move(status));
            return ptr;
        }
        virtual std::shared_ptr<Object>
        emplace_object(Object object, Coordinate coordinate, Velocity velocity = Velocity(0, 0), std::list<Force> forces = std::list<Force>())
        {
            auto ptr = std::make_shared<Object>(std::move(object));
            _task_queue.emplace(QueueBehavior::Emplace, ptr, ObjectStatus(coordinate, velocity, forces));
            return ptr;
        }
        virtual void erase_object(std::shared_ptr<Object> objptr)
        {
            _task_queue.emplace(QueueBehavior::Erase, objptr, ObjectStatus(Coordinate(0, 0)));
        }
        virtual bool exist_object(std::shared_ptr<Object> objptr) const
        {
            std::shared_lock<std::shared_mutex> lock(_objs_mutex);
            return _objects.find(objptr) != _objects.end();
        }

        virtual const ObjectStatus &object_status(std::shared_ptr<Object> objptr) const
        {
            std::shared_lock<std::shared_mutex> lock(_objs_mutex);
            return _objects.at(objptr);
        }

        virtual Time &time() { return *_time_ptr; }

    protected:
        virtual void on_tick(double ticked_time) = 0;
        virtual void run_task()
        {
            // std::unique_lock<std::shared_mutex> lock(_objs_mutex);
            while (!_task_queue.empty())
            {
                auto &&[behavior, object_ptr, status] = _task_queue.front();
                switch (behavior)
                {
                case QueueBehavior::Emplace:
                    _objects.emplace(std::move(object_ptr), std::move(status));
                    break;
                case QueueBehavior::Erase:
                    _objects.erase(object_ptr);
                    break;
                default:
                    break;
                }
                _task_queue.pop();
            }
        }

        std::unique_ptr<Time> _time_ptr = nullptr;
        std::unordered_map<std::shared_ptr<Object>, ObjectStatus> _objects;
        std::queue<std::tuple<QueueBehavior, std::shared_ptr<Object>, ObjectStatus>> _task_queue;
        mutable std::shared_mutex _objs_mutex;

    private:
        static void callback_tick(Space *p_this, double ticked_time)
        {
            p_this->on_tick(ticked_time);
        }
    };
} // namespace meophys