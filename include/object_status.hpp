#pragma once

#include <vector>
#include <numeric>

namespace meophys
{
    class ObjectStatus
    {
    public:
        ObjectStatus(Coordinate coordinate, Velocity velocity = Velocity(0, 0), std::vector<Force> forces = std::vector<Force>())
            : _coordinate(std::move(coordinate)), _velocity(std::move(velocity)), _forces(std::move(forces)) {}
        ObjectStatus(const ObjectStatus &) = default;
        ObjectStatus(ObjectStatus &&) = default;
        ~ObjectStatus() = default;
        // 坐标，单位m
        Coordinate &coordinate() noexcept { return _coordinate; }
        const Coordinate &coordinate() const noexcept { return _coordinate; }

        // 速率，单位m/s
        Velocity &velocity() noexcept { return _velocity; }
        const Velocity &velocity() const noexcept { return _velocity; }

        // 所受的力，单位N
        const Force sum_of_forces() const
        {
            // return std::reduce(_forces.cbegin(), _forces.cend(), Force(0, 0));
            Force sum(0, 0);
            for (auto &&f : _forces)
            {
                sum += f;
            }
            return sum;
        }
        Force &exert_force(Force force)
        {
            return _forces.emplace_back(std::move(force));
        }

    private:
        Coordinate _coordinate;     // 坐标，单位m
        Velocity _velocity;         // 速率，单位m/s
        std::vector<Force> _forces; // 所受的力，单位N
    };
} // namespace meophys