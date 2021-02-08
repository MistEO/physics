#pragma once

#include <vector>
#include <numeric>
#include <shared_mutex>
#include "define.h"

namespace meophys
{
    class ObjectStatus
    {
    public:
        // 坐标，单位m
        const Coordinate get_coordinate() const
        {
            std::shared_lock<std::shared_mutex> lock(_coordinate_mutex);
            return _coordinate;
        }
        void set_coordinate(Coordinate coordinate)
        {
            std::unique_lock<std::shared_mutex> lock(_coordinate_mutex);
            _coordinate = std::move(coordinate);
        }
        void set_coordinate(Coordinate::first_type vx, Coordinate::second_type vy)
        {
            std::unique_lock<std::shared_mutex> lock(_coordinate_mutex);
            _coordinate = Coordinate(vx, vy);
        }

        // 速率，单位m/s
        const Velocity get_velocity() const
        {
            std::shared_lock<std::shared_mutex> lock(_velocity_mutex);
            return _velocity;
        }
        void set_velocity(Velocity velocity)
        {
            std::unique_lock<std::shared_mutex> lock(_velocity_mutex);
            _velocity = std::move(velocity);
        }
        void set_velocity(Velocity::first_type vx, Velocity::second_type vy)
        {
            std::unique_lock<std::shared_mutex> lock(_velocity_mutex);
            _velocity = Velocity(vx, vy);
        }

        // 所受的力，单位N
        Force ObjectStatus::sum_of_forces() const
        {
            std::shared_lock<std::shared_mutex> lock(_force_mutex);
            return std::reduce(_forces.cbegin(), _forces.cend(), Force(0, 0));
        }
        Force &ObjectStatus::exert_force(Force force)
        {
            std::unique_lock<std::shared_mutex> lock(_force_mutex);
            return _forces.emplace_back(std::move(force));
        }

    private:
        Coordinate _coordinate;     // 坐标，单位m
        Velocity _velocity;         // 速率，单位m/s
        std::vector<Force> _forces; // 所受的力，单位N

        mutable std::shared_mutex _coordinate_mutex;
        mutable std::shared_mutex _velocity_mutex;
        mutable std::shared_mutex _force_mutex;
    };
} // namespace meophys