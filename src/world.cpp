#include "world.h"

#include <cmath>
#include "object.hpp"

using namespace meophys;

void World::on_tick(double ticked_time)
{
    std::shared_lock<std::shared_mutex> rdlock(_objs_mutex);
    auto cur_objects = _objects;
    const auto pre_objects = _objects;
    rdlock.unlock();
    std::unique_lock<std::mutex> exp_lock(_explosion_mutex);
    // 直接清空当前爆炸容器，本次tick全部计算完成
    const auto cur_explosions = std::move(_explosions);
    exp_lock.unlock();

    for (auto &&[obj, status] : cur_objects)
    {
        auto name = obj->name(); // for debugger
        // 碰撞的计算
        Force collision_force(0, 0);
        for (auto &&[ano_obj, ano_status] : pre_objects)
        {
            auto ano_name = ano_obj->name(); // for debugger

            if (obj == ano_obj)
            {
                continue;
            }

            // 计算两两之间距离，若小于半径和，则视为碰撞
            double obj_distance = meophys::distance(status.coordinate(), ano_status.coordinate());
            if (obj_distance < (obj->radius() + ano_obj->radius() + FloatDiff))
            {
                auto &&[x1, y1] = status.coordinate() - ano_status.coordinate();

                // 非对心碰撞，速度计算，
                {
                    auto &e = obj->elasticity();
                    auto v1 = status.velocity();
                    auto v2 = ano_status.velocity();
                    auto &m1 = obj->mass();
                    auto &m2 = ano_obj->mass();

                    // 不应对v1、v2做动量计算，而是对v1、v2在法线上的分量速度做动量计算
                    Velocity v1t = Velocity(0, 0); // v1在法线/切线坐标系上的速度分量
                    double v1_cosa = 0;
                    double v1t_x = 0;
                    if (v1 == Velocity(0, 0))
                    {
                        v1t = Velocity(0, 0);
                    }
                    else
                    {
                        auto &&[v1_x2, v1_y2] = v1;
                        // cosα = ab / | a || b |= (x1x2 + y1y2) / (√(x1²+ y1²)√(x2²+ y2²))
                        v1_cosa = (x1 * v1_x2 + y1 * v1_y2) / (std::sqrt(std::pow(x1, 2) + std::pow(y1, 2)) * std::sqrt(std::pow(v1_x2, 2) + std::pow(v1_y2, 2)));
                        v1t = v1_cosa * v1;
                        // 切线上的速度分量不参与碰撞，稍后直接还原到x/y坐标系中
                        // 而法线上的速度分量参与碰撞后，再还原到x/y坐标系中
                        v1t_x = v1t.first;
                        v1t.first = 0;
                    }

                    Velocity v2t = Velocity(0, 0); // v2在法线/切线坐标系上的速度分量
                    double v2_cosa = 0;
                    if (v2 == Velocity(0, 0))
                    {
                        v2t = Velocity(0, 0);
                    }
                    else
                    {
                        auto &&[v2_x2, v2_y2] = v2;
                        // cosα = ab / | a || b |= (x1x2 + y1y2) / (√(x1²+ y1²)√(x2²+ y2²))
                        v2_cosa = (x1 * v2_x2 + y1 * v2_y2) / (std::sqrt(std::pow(x1, 2) + std::pow(y1, 2)) * std::sqrt(std::pow(v2_x2, 2) + std::pow(v2_y2, 2)));
                        v2t = v2_cosa * v2;
                        // 切线上的速度分量不参与碰撞，别的物体的不用还原了
                        v2t.first = 0;
                    }

                    // Elastic Collision
                    // v1' = ( (m1-e*m2)*v1+(1+e)*m2*v2 ) / ( m1+m2 )
                    // 这里计算出来的同样也是法线/切线坐标系上的速度，这里的vt_x肯定是0
                    auto &&[vt_x, vt_y] = (((m1 - e * m2) * v1t + (1 + e) * m2 * v2t) / (m1 + m2));

                    // 将法线上的速度，转换成x/y坐标系中的速度
                    // 计算法线与y轴的cos，y轴即(0, 1)
                    // cosα = ab / | a || b |= (x1x2 + y1y2) / (√(x1²+ y1²)√(x2²+ y2²)) = y1 / √(x1²+ y1²)
                    double cosa_y = std::fabs(y1) / std::sqrt(std::pow(x1, 2) + std::pow(y1, 2));
                    // sin²a + cos²a = 1
                    double sina_y = std::sqrt(1 - std::pow(cosa_y, 2));
                    double v_y = cosa_y * vt_y; // + sina_y * v1t_x;
                    double v_x = sina_y * vt_y; //+ cosa_y * v1t_x;

                    status.velocity() = Velocity(v_x, v_y);
                }

                // 非对心碰撞，受力计算。需要计算别人对自己的力 + 自己对别人的力的反作用力
                {
                    // 别人对自己的力
                    {
                        auto &&[f_x2, f_y2] = ano_status.sum_of_forces();
                        double f_cosa = (x1 * f_x2 + y1 * f_y2) / (std::sqrt(std::pow(x1, 2) + std::pow(y1, 2)) * std::sqrt(std::pow(f_x2, 2) + std::pow(f_y2, 2)));

                        // F压力 = F2 * cosθ             θ ∈ [ 0 - 90°), cosθ ∈ (0, 1)
                        // F压力 = 0                    θ ∈ [90 - 180°], cosθ ∈ [-1, 0], 1
                        if (0 < f_cosa && f_cosa < 1.0)
                        {
                            double fp_value = meophys::abs(ano_status.sum_of_forces()) * f_cosa;
                            double fp_x = fp_value / obj_distance * (status.coordinate().first - ano_status.coordinate().first);
                            double fp_y = fp_value / obj_distance * (status.coordinate().second - ano_status.coordinate().second);
                            collision_force += Force(fp_x, fp_y);
                        }
                        else if (f_cosa == 1.0 || (-1.0 <= f_cosa && f_cosa <= 0))
                        {
                            // f_pressure = 0, do nothing
                        }
                        else
                        {
                            throw std::runtime_error("cosθ error:" + std::to_string(f_cosa));
                        }
                    }

                    // 自己对别人的力的反作用力
                    {
                        auto &&[neg_x1, neg_y1] = ano_status.coordinate() - status.coordinate();
                        auto &&[f_x2, f_y2] = status.sum_of_forces();
                        double f_cosa = (neg_x1 * f_x2 + neg_y1 * f_y2) / (std::sqrt(std::pow(neg_x1, 2) + std::pow(neg_y1, 2)) * std::sqrt(std::pow(f_x2, 2) + std::pow(f_y2, 2)));

                        // F压力 = F2 * cosθ             θ ∈ [ 0 - 90°), cosθ ∈ (0, 1)
                        // F压力 = 0                    θ ∈ [90 - 180°], cosθ ∈ [-1, 0], 1
                        if (0 < f_cosa && f_cosa < 1.0)
                        {
                            double fp_value = meophys::abs(status.sum_of_forces()) * f_cosa;
                            double fp_x = fp_value / obj_distance * (ano_status.coordinate().first - status.coordinate().first);
                            double fp_y = fp_value / obj_distance * (ano_status.coordinate().second - status.coordinate().second);
                            // 注意是反作用力，负的
                            collision_force += (-Force(fp_x, fp_y));
                        }
                        else if (f_cosa == 1.0 || (-1.0 <= f_cosa && f_cosa <= 0))
                        {
                            // f_pressure = 0, do nothing
                        }
                        else
                        {
                            throw std::runtime_error("cosθ error:" + std::to_string(f_cosa));
                        }
                    }
                }
            }
        }

        auto &&pf = status.emplace_force(std::move(collision_force));
        auto sum_of_forces = status.sum_of_forces();
        pf = Force(0, 0);

        // 爆炸的计算
        for (auto &&[coor, energy] : cur_explosions)
        {
            // E' = k * E / r²，该公式待优化
            // E = ½mv², v = sqrt(2E / m)
            auto r2 = distance_squared(coor, status.coordinate());
            auto e = ExplosionAttention * energy / r2;
            auto v_value = std::sqrt(2 * e / obj->mass());

            double r = std::sqrt(r2);
            if (r == 0)
            {
                continue;
            }
            double v_x = (status.coordinate().first - coor.first) * v_value / r;
            double v_y = (status.coordinate().second - coor.second) * v_value / r;
            status.velocity() += Velocity(v_x, v_y);
        }

        // a = F / m
        Acceleration acc = sum_of_forces / obj->mass();
        // x = Vt + ½at²
        Displacement dp = status.velocity() * ticked_time + 0.5 * acc * std::pow(ticked_time, 2);

        // 算一下如果直接走，会不会穿模
        Coordinate will_go = status.coordinate() + dp;
        auto &&[will_go_x, will_go_y] = will_go;
        bool top_out = will_go_y + obj->radius() > _boundary.top - FloatDiff;
        bool left_out = will_go_x - obj->radius() < _boundary.left + FloatDiff;
        bool bottom_out = will_go_y - obj->radius() < _boundary.bottom + FloatDiff;
        bool right_out = will_go_x + obj->radius() > _boundary.right - FloatDiff;

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
            friction.first = direction * obj->friction() * sum_of_forces.second;
            // 摩擦力不对静止物体做功
            if (std::fabs(friction.first) / obj->mass() * ticked_time > std::fabs(v_x))
            {
                friction.first = 0;
                v_x = 0;
            }
        }
        if (left_out || right_out)
        {
            int direction = std::fabs(v_y * ticked_time) <= PlanckLength ? 0 : (v_y > 0 ? 1 : -1);
            friction.second = direction * obj->friction() * sum_of_forces.first;
            // 摩擦力不对静止物体做功
            if (std::fabs(friction.second) / obj->mass() * ticked_time > std::fabs(v_y))
            {
                friction.second = 0;
                v_y = 0;
            }
        }

        acc = (sum_of_forces + friction) / obj->mass();
        // 若已经贴在边界上了（或即将穿模），且力朝边界外；则该方向上不提供加速度
        if ((top_out && sum_of_forces.second > 0) ||
            (bottom_out && sum_of_forces.second < 0))
        {
            acc.second = 0;
        }
        if ((left_out && sum_of_forces.first > 0) ||
            (right_out && sum_of_forces.first < 0))
        {
            acc.first = 0;
        }
        // x = Vt + ½at²
        dp = status.velocity() * ticked_time + 0.5 * acc * std::pow(ticked_time, 2);
        will_go = status.coordinate() + dp;

        std::tie(v_x, v_y) = status.velocity() + acc * ticked_time;

        if (top_out)
        {
            will_go_y = _boundary.top - obj->radius();
        }
        else if (bottom_out)
        {
            will_go_y = _boundary.bottom + obj->radius();
        }

        if (right_out)
        {
            will_go_x = _boundary.right - obj->radius();
        }
        else if (left_out)
        {
            will_go_x = _boundary.left + obj->radius();
        }
        status.coordinate() = will_go;

        // E = ½mv²
        if (top_out || bottom_out)
        {
            v_y = -v_y * std::pow(obj->elasticity(), 2);
        }
        if (left_out || right_out)
        {
            v_x = -v_x * std::pow(obj->elasticity(), 2);
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

    std::unique_lock<std::shared_mutex> wrlock(_objs_mutex);
    _objects = std::move(cur_objects);
}