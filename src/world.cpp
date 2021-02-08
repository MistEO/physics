#include "world.h"

#include <cmath>
#include "object.hpp"

using namespace meophys;

void World::on_tick(double ticked_time)
{
    std::shared_lock<std::shared_mutex> rdlock(_objs_mutex);
    auto temp_objects = _objects;
    rdlock.unlock();
    for (auto &&[obj, status] : temp_objects)
    {
        // a = F / m
        Acceleration acc = status.sum_of_forces() / obj.mass();
        // x = Vt + ½at²
        Displacement dp = status.velocity() * ticked_time + 0.5 * acc * std::pow(ticked_time, 2);

        // 算一下如果直接走，会不会穿模
        Coordinate will_go = status.coordinate() + dp;
        auto &&[will_go_x, will_go_y] = will_go;
        bool top_out = will_go_y + obj.radius() > _boundary.top - FloatDiff;
        bool left_out = will_go_x - obj.radius() < _boundary.left + FloatDiff;
        bool bottom_out = will_go_y - obj.radius() < _boundary.bottom + FloatDiff;
        bool right_out = will_go_x + obj.radius() > _boundary.right - FloatDiff;

        // 如果都不穿模，直接走就行了
        if (!(top_out || bottom_out || left_out || right_out))
        {
            status.coordinate() = will_go;
            // Vt = V0 + at
            status.velocity() += acc * ticked_time;
            continue;
        }

        // 加速度 a = F / m
        // 摩擦力 F = u * Fn
        double v_x = 0, v_y = 0;
        std::tie(v_x, v_y) = status.velocity();
        Force friction(0, 0);
        if (top_out || bottom_out)
        {
            int direction = std::fabs(v_x * ticked_time) <= PlanckLength ? 0 : (v_x > 0 ? 1 : -1);
            friction.first = direction * obj.friction() * status.sum_of_forces().second;
        }
        if (left_out || right_out)
        {
            int direction = std::fabs(v_y * ticked_time) <= PlanckLength ? 0 : (v_y > 0 ? 1 : -1);
            friction.second = direction * obj.friction() * status.sum_of_forces().first;
        }

        acc = (status.sum_of_forces() + friction) / obj.mass();
        // 若已经贴在边界上了（或即将穿模），且力朝边界外；则该方向上不提供加速度
        if ((top_out && status.sum_of_forces().second > 0) ||
            (bottom_out && status.sum_of_forces().second < 0))
        {
            acc.second = 0;
        }
        if ((left_out && status.sum_of_forces().first > 0) ||
            (right_out && status.sum_of_forces().first < 0))
        {
            acc.first = 0;
        }
        // x = Vt + ½at²
        dp = status.velocity() * ticked_time + 0.5 * acc * std::pow(ticked_time, 2);
        will_go = status.coordinate() + dp;

        if (top_out)
        {
            will_go_y = _boundary.top - obj.radius();
        }
        else if (bottom_out)
        {
            will_go_y = _boundary.bottom + obj.radius();
        }

        if (right_out)
        {
            will_go_x = _boundary.right - obj.radius();
        }
        else if (left_out)
        {
            will_go_x = _boundary.left + obj.radius();
        }
        status.coordinate() = will_go;

        // E = ½mv²
        std::tie(v_x, v_y) = status.velocity() + acc * ticked_time;
        if (top_out || bottom_out)
        {
            v_y = -v_y * std::pow(obj.elasticity(), 2);
        }
        if (left_out || right_out)
        {
            v_x = -v_x * std::pow(obj.elasticity(), 2);
        }
        if (std::fabs(v_x * ticked_time) < PlanckLength)
        {
            v_x = 0;
        }
        if (std::fabs(v_y * ticked_time) < PlanckLength)
        {
            v_y = 0;
        }

        // Vt = V0 + at
        status.velocity() = Velocity(v_x, v_y);
    }
    // TODO: 物体碰撞
    // 计算两两之间距离，若小于半径和，则视为碰撞
    auto collision_objects = temp_objects;
    for (auto &&[obj, status] : collision_objects)
    {
        for (auto &&[ano_obj, ano_status] : temp_objects)
        {
            if (obj == ano_obj)
            {
                continue;
            }

            // 发生碰撞
            if (distance(status.coordinate(), ano_status.coordinate()) < (obj.radius() + ano_obj.radius() + FloatDiff))
            {
                double e = (obj.elasticity() + ano_obj.elasticity()) / 2;
                auto v1 = status.velocity();
                auto v2 = ano_status.velocity();
                auto &m1 = obj.mass();
                auto &m2 = ano_obj.mass();

                // Elastic Collision
                // v1' = ( (m1-e*m2)*v1+(1+e)*m2*v2 ) / ( m1+m2 )
                // v2' = ( (1+e)*m1*v1+(m2-e*m1)*v2) ) / ( m1+m2 )
                auto v_obj = ((m1 - e * m2) * v1 + (1 + e) * m2 * v2) / (m1 + m2);

                status.velocity() = v_obj;
            }
        }
    }

    std::unique_lock<std::shared_mutex> wrlock(_objs_mutex);
    _objects = std::move(temp_objects);
}