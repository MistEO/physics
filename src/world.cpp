#include "world.h"

#include <cmath>
#include "object.h"

using namespace meophys;

void World::on_tick(double ticked_time)
{
    std::shared_lock<std::shared_mutex> rdlock(_objs_mutex);
    auto temp_objects = _objects;
    rdlock.unlock();
    for (auto &&[obj, coor] : temp_objects)
    {
        if (obj == nullptr)
        {
            continue;
        }

        // a = F / m
        Acceleration acc = obj->sum_of_forces() / obj->mass();
        // x = Vt + ½at²
        Displacement dp = obj->velocity() * ticked_time + 0.5 * acc * std::pow(ticked_time, 2);

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
            obj->velocity() += acc * ticked_time;
            continue;
        }

        // 加速度 a = F / m
        // 摩擦力 F = u * Fn
        // TODO: 摩擦力计算错误
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

        acc = (obj->sum_of_forces() + friction) / obj->mass();
        // 若已经贴在边界上了（或即将穿模），且力朝边界外；则该方向上不提供加速度
        if ((top_out && obj->sum_of_forces().second > PlanckLength) ||
            (bottom_out && obj->sum_of_forces().second < -PlanckLength))
        {
            acc.second = 0;
        }
        if ((left_out && obj->sum_of_forces().first > PlanckLength) ||
            (right_out && obj->sum_of_forces().first < -PlanckLength))
        {
            acc.first = 0;
        }
        // x = Vt + ½at²
        dp = obj->velocity() * ticked_time + 0.5 * acc * std::pow(ticked_time, 2);
        will_go = coor + dp;

        if (top_out)
        {
            will_go_y = _boundary.top;
        }
        else if (bottom_out)
        {
            will_go_y = _boundary.bottom;
        }

        if (right_out)
        {
            will_go_x = _boundary.right;
        }
        else if (left_out)
        {
            will_go_x = _boundary.left;
        }
        coor = will_go;

        // Vt = V0 + at
        obj->velocity() += acc * ticked_time;
        // E = ½mv²
        if (top_out || bottom_out)
        {
            v_y = -v_y * std::pow(obj->elasticity(), 2);
        }
        if (left_out || right_out)
        {
            v_x = -v_x * std::pow(obj->elasticity(), 2);
        }
    }
    std::unique_lock<std::shared_mutex> wrlock(_objs_mutex);
    _objects = std::move(temp_objects);
}