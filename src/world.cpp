#include "world.h"

#include <cmath>
#include "object.h"

using namespace meophys;

World::World()
    : Space()
{
    // _time_ptr = (std::make_unique<Time>(world_callback_when_ticktime, this));
}

void World::callback_when_ticktime()
{
    for (auto &&[obj, coor] : _objects)
    {
        if (obj == nullptr)
        {
            return;
        }

        // a = F / m
        Acceleration acc = obj->sum_of_forces() / obj->mass();
        // x = Vt + ½at²
        Displacement dp = obj->velocity() * PlankTime + 0.5 * acc * std::pow(PlankTime, 2);

        // 算一下如果直接走，会不会穿模
        Coordinate will_go = coor + dp;
        auto &&[will_go_x, will_go_y] = will_go;
        bool top_out = will_go_y >= _boundary.top - PlanckLength;
        bool left_out = will_go_x <= _boundary.left + PlanckLength;
        bool bottom_out = will_go_y <= _boundary.bottom + PlanckLength;
        bool right_out = will_go_x >= _boundary.right - PlanckLength;

        // 如果都不穿模，直接走就行了
        if (!(top_out || bottom_out || left_out || right_out))
        {
            coor = will_go;
            // Vt = V0 + at
            obj->velocity() = obj->velocity() + acc * PlankTime;
            continue;
        }

        // 加速度 a = F / m
        // 摩擦力 F = u * Fn
        auto &&[v_x, v_y] = obj->velocity();
        Force friction(0, 0);
        if (top_out || bottom_out)
        {
            int direction = std::fabs(v_x) <= PlanckLength ? 0 : (v_x > PlanckLength ? 1 : -1);
            friction.second = direction * obj->friction() * obj->sum_of_forces().second;
        }
        if (left_out || right_out)
        {
            int direction = std::fabs(v_y) <= PlanckLength ? 0 : (v_y > PlanckLength ? 1 : -1);
            friction.first = direction * obj->friction() * obj->sum_of_forces().first;
        }
    
    }
}