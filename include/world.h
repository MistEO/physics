#pragma once

#include <vector>
#include "define.h"
#include "space.hpp"

namespace meophys
{
    class World : public Space
    {
    public:
        struct Boundary
        {
            int top = 1;
            int left = 0;
            int bottom = 0;
            int right = 1;
        };

    public:
        World() = default;
        World(const World &) = delete;
        World(World &&) = delete;
        virtual ~World() = default;

        void set_boundary(int top, int left, int bottom, int right)
        {
            _boundary.top = top;
            _boundary.left = left;
            _boundary.bottom = bottom;
            _boundary.right = right;
        }
        Boundary &boundary() { return _boundary; }
        void explode(Coordinate coordinate, long long energy)
        {
            std::unique_lock<std::mutex> lock(_explosion_mutex);
            _explosions.emplace_back(std::move(coordinate), std::move(energy));
        }

        World &operator=(const World &) = delete;
        World &operator=(World &&) = delete;

    protected:
        virtual void on_tick(double ticked_time, const ObjectAndStatusMap &pre_objs, ObjectAndStatusMap &cur_objs) override;

    private:
        Boundary _boundary;
        constexpr static long double ExplosionAttention = 1.0;
        std::vector<std::pair<Coordinate, long long>> _explosions;
        std::mutex _explosion_mutex;
    };
} // namespace meophys