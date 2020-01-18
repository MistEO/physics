#include "world.h"

#include <cmath>
#include <iostream>

World::~World()
{
    for (auto& g : gravity_vector) {
        delete g;
        g = nullptr;
    }
}

void World::set_gravity(double gravity)
{
    this->gravity = gravity;
}

void World::push_object(Object* object)
{
    if (object == nullptr) {
        std::cerr << "the object is empty" << std::endl;
        return;
    }
    objects.push_back(object);
    Force* force = new Force(0, gravity * object->mass);
    object->forces.push_back(force);
    gravity_vector.push_back(force);
}

void World::set_boundary(Coordinate bottomleft, Coordinate topright)
{
    boundary = std::make_pair(bottomleft, topright);
}

void World::set_boundary(std::pair<Coordinate, Coordinate> boundary)
{
    this->boundary = boundary;
}

std::pair<Coordinate, Coordinate> World::get_boundary() const
{
    return boundary;
}

void World::tick_object(Object* object, double time)
{
    Coordinate will_go = object->will_go(time);
    bool bottom_out = will_go.second <= boundary.first.second + DOUBLE_DIFF;
    bool left_out = will_go.first <= boundary.first.first + DOUBLE_DIFF;
    bool top_out = will_go.second >= boundary.second.second - DOUBLE_DIFF;
    bool right_out = will_go.first >= boundary.second.first - DOUBLE_DIFF;
    // 如果都不越界，直接走就行了
    if (!(top_out || bottom_out || left_out || right_out)) {
        object->tick(time);
        return;
    }

    // 加速度 a = F / m
    // 摩擦力 F = u * Fn
    Force friction(0, 0);
    if (top_out || bottom_out) {
        int sign = fabs(object->velocity.first) <= DOUBLE_DIFF ? 0 : (object->velocity.first > DOUBLE_DIFF ? 1 : -1);
        friction.first = object->friction * object->sum_of_forces().second * sign;
    }
    if (left_out || right_out) {
        int sign = fabs(object->velocity.second) <= DOUBLE_DIFF ? 0 : (object->velocity.second > DOUBLE_DIFF ? 1 : -1);
        friction.second = object->friction * object->sum_of_forces().first * sign;
    }
    Acceleration acceleration = (object->sum_of_forces() + friction) / object->mass;
    // 若已经贴在边界上了（或要出边界了），力朝边界外，则该方向上力不提供加速度
    if ((top_out && object->sum_of_forces().second > DOUBLE_DIFF)
        || (bottom_out && object->sum_of_forces().second < -DOUBLE_DIFF)) {
        acceleration.second = 0;
    }
    if ((left_out && object->sum_of_forces().first > DOUBLE_DIFF)
        || (right_out && object->sum_of_forces().second < -DOUBLE_DIFF)) {
        acceleration.first = 0;
    }
    // x = Vt + ½at²
    Displacement displacement = object->velocity * time + 0.5 * acceleration * time * time;
    will_go = object->coordinate + displacement;

    if (top_out) {
        will_go.second = boundary.second.second;
    } else if (bottom_out) {
        will_go.second = boundary.first.second;
    }
    if (right_out) {
        will_go.first = boundary.second.first;
    } else if (left_out) {
        will_go.first = boundary.first.first;
    }
    object->coordinate = will_go;

    // Vt = V0 + at
    object->velocity = object->velocity + acceleration * time;
    // E = ½mv²
    if (top_out || bottom_out) {
        object->velocity.second = -object->velocity.second * std::pow(object->elasticity, 2);
    }
    if (left_out || right_out) {
        object->velocity.first = -object->velocity.first * std::pow(object->elasticity, 2);
    }
}