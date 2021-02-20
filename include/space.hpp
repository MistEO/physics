#pragma once

#include <memory>
#include <unordered_map>
#include <shared_mutex>
#include <queue>
#include <tuple>
#include <atomic>

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
        using ObjectAndStatusMap = std::unordered_map<std::shared_ptr<Object>, ObjectStatus>;

        Space() : _time_ptr(std::make_unique<Time>(callback_tick, this))
        {
        }
        Space(const Space &) = delete;
        Space(Space &&) = delete;

        virtual ~Space() = default;

        std::shared_ptr<Object>
        emplace_object(Object object, ObjectStatus status)
        {
            auto ptr = std::make_shared<Object>(std::move(object));
            std::unique_lock<std::mutex> task_lock(_task_mutex);
            if (_is_tick_running)
            {
                _task_queue.emplace(QueueBehavior::Emplace, ptr, std::move(status));
            }
            else
            {
                std::unique_lock<std::shared_mutex> obj_lock(_objs_mutex);
                task_lock.unlock();
                _objects.emplace(ptr, std::move(status));
            }
            return ptr;
        }
        std::shared_ptr<Object>
        emplace_object(Object object, Coordinate coordinate, Velocity velocity = Velocity(0, 0), std::list<Force> forces = std::list<Force>())
        {
            auto ptr = std::make_shared<Object>(std::move(object));
            std::unique_lock<std::mutex> task_lock(_task_mutex);
            if (_is_tick_running)
            {
                _task_queue.emplace(QueueBehavior::Emplace, ptr, ObjectStatus(coordinate, velocity, forces));
            }
            else
            {
                std::unique_lock<std::shared_mutex> obj_lock(_objs_mutex);
                task_lock.unlock();
                _objects.emplace(ptr, ObjectStatus(coordinate, velocity, forces));
            }
            return ptr;
        }
        void erase_object(std::shared_ptr<Object> objptr)
        {
            std::unique_lock<std::mutex> task_lock(_task_mutex);
            if (_is_tick_running)
            {
                _task_queue.emplace(QueueBehavior::Erase, objptr, ObjectStatus(Coordinate(0, 0)));
            }
            else
            {
                std::unique_lock<std::shared_mutex> obj_lock(_objs_mutex);
                task_lock.unlock();
                _objects.erase(objptr);
            }
        }
        bool exist_object(std::shared_ptr<Object> objptr) const
        {
            std::shared_lock<std::shared_mutex> lock(_objs_mutex);
            return _objects.find(objptr) != _objects.end();
        }

        const ObjectStatus &object_status(std::shared_ptr<Object> objptr) const
        {
            std::shared_lock<std::shared_mutex> lock(_objs_mutex);
            return _objects.at(objptr);
        }

        Time &time() { return *_time_ptr; }

        Space &operator=(const Space &) = delete;
        Space &operator=(Space &&) = delete;

    protected:
        virtual void on_tick(double ticked_time)
        {
            std::unique_lock<std::mutex> task_lock(_task_mutex);
            _is_tick_running = true;
            run_task();
            task_lock.unlock();

            std::shared_lock<std::shared_mutex> rdlock(_objs_mutex);
            auto cur_objs = _objects;
            const auto pre_objs = _objects;
            rdlock.unlock();

            on_tick(ticked_time, pre_objs, cur_objs);

            std::unique_lock<std::shared_mutex> wrlock(_objs_mutex);
            _objects = std::move(cur_objs);
            wrlock.unlock();

            task_lock.lock();
            _is_tick_running = false;
            task_lock.unlock();
        }
        virtual void on_tick(double ticked_time, const ObjectAndStatusMap &pre_objs, ObjectAndStatusMap &cur_objs) = 0;

        void run_task()
        {
            std::unique_lock<std::shared_mutex> wrlock(_objs_mutex);

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
        mutable std::mutex _task_mutex;
        mutable std::atomic_bool _is_tick_running = false;

    private:
        static void callback_tick(Space *p_this, double ticked_time)
        {
            p_this->on_tick(ticked_time);
        }
    };
} // namespace meophys