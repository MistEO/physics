#include "space.h"

#include <cmath>
#ifndef DOUBLE_DIFF
#define DOUBLE_DIFF 1e-8
#endif

void Space::push_object(Object* object)
{
    objects.push_back(object);
}

void Space::push_boundary(const Boundary& boundary)
{
    boundaries.push_back(boundary);
}

void Space::tick(double time)
{
    for (const auto& object : objects) {

        Coordinate will_go = object->will_go(time);
        Boundary out_boundary(BoundaryNone, 0);
        for (const auto& boundary : boundaries) {
            double x0 = object->coordinate.first, y0 = object->coordinate.second,
                   x1 = will_go.first, y1 = will_go.second;
            // 前后两次分别在边界线两侧，则说明越界
            if ((boundary.first == BoundaryX && (y0 - boundary.second) * (y1 - boundary.second) < DOUBLE_DIFF)
                || (boundary.second == BoundaryY && (x0 - boundary.second) * (x1 - boundary.second) < DOUBLE_DIFF)) {
                out_boundary = boundary;
                break;
            }
        }
        if (out_boundary.first == BoundaryNone) {
            object->tick(time);
            continue;
        }

        // 加速度 a = F / m
        // 摩擦力 F = u * Fn
        Force friction(0, 0);
        if (out_boundary.first == BoundaryX) {
            friction.first = object->friction * object->sum_of_forces().second * (object->velocity.first > 0 ? -1 : 1);
        } else {
            friction.second = object->friction * object->sum_of_forces().first * (object->velocity.second > 0 ? -1 : 1);
        }
        Acceleration acceleration = (object->sum_of_forces() - friction) / object->mass;
        // 若已经贴在边界上了，切初速度为0，则不提供加速度
        if (out_boundary.first == BoundaryX
            && fabs(object->coordinate.second - out_boundary.second) < DOUBLE_DIFF
            && object->velocity.second < DOUBLE_DIFF) {
            acceleration.second = 0;
        }
        if (out_boundary.first == BoundaryY
            && fabs(object->coordinate.first - out_boundary.second) < DOUBLE_DIFF
            && fabs(object->velocity.first) < DOUBLE_DIFF) {
            acceleration.first = 0;
        }

        // x = Vt + ½at²
        Displacement displacement = object->velocity * time + 0.5 * acceleration * time * time;
        will_go = object->coordinate + displacement;

        if (out_boundary.first == BoundaryX) {
            will_go.second = 0;
        } else {
            will_go.first = 0;
        }
        object->coordinate = will_go;

        // Vt = V0 + at
        object->velocity = object->velocity + acceleration * time;
        // E = ½mv²
        if (out_boundary.first == BoundaryX) {
            object->velocity.second = -object->velocity.second * std::pow(object->elasticity, 2);
        } else {
            object->velocity.first = -object->velocity.first * std::pow(object->elasticity, 2);
        }
    }
}