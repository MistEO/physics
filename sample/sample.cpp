#include <iostream>
#include <string>
#include <cmath>
#include <cstdarg>
#include <unistd.h>

#include "physics.h"

using namespace meophys;

void world_and_ball();
void world_and_ball_test();
void world_and_many_ball();
void world_and_boom();
void interstellar_and_planet();
void print_loop(Space *space, const std::vector<std::shared_ptr<Object>> &objects, double scale = 1.0);

int main()
{
    world_and_boom();
    // world_and_ball_test();
    // world_and_many_ball();
    // interstellar_and_planet();
    return 0;
}

void interstellar_and_planet()
{
    Interstellar space;

    auto earth = space.emplace_object(Object("Earth", 5.965e24), Coordinate(0, 0));
    auto month = space.emplace_object(Object("Month", 7.349e22), Coordinate(384403.9e3, 0), Velocity(0, 1.023e3));

    space.time().timeflow() = 1000000;
    space.time().start();

    print_loop(&space, {earth, month}, 1e-8);
}

void world_and_ball()
{
    World world;
    world.set_boundary(11, -10, 0, 50);

    Object ball("1", 1);
    ball.elasticity() = 0.9;
    ball.friction() = 0.5;
    ObjectStatus ball_status(Coordinate(1, 10), Velocity(5, 0), {Force(0, -9.8)});

    Object ball_2("2", 1);
    ball_2.elasticity() = 0.9;
    ball_2.friction() = 0.5;
    ObjectStatus ball_2_status(Coordinate(20, 10), Velocity(-5, 0), {Force(0, -9.8)});

    auto ball_ptr = world.emplace_object(std::move(ball), std::move(ball_status));
    auto ball_2_ptr = world.emplace_object(std::move(ball_2), std::move(ball_2_status));

    world.time().start();

    print_loop(&world, {ball_ptr, ball_2_ptr});
}

void world_and_ball_test()
{
    World world;
    world.set_boundary(11, 0, 0, 50);

    Object ball("1", 1);
    ball.elasticity() = 0.9;
    ball.friction() = 0.1;
    ObjectStatus ball_status(Coordinate(10, 10), Velocity(0, 0), {Force(0.1, -9.8)});

    Object ball_2("2", 1);
    ball_2.elasticity() = 0.9;
    ball_2.friction() = 0.1;
    ObjectStatus ball_2_status(Coordinate(9, 1), Velocity(0, 0), {Force(0, -9.8)});

    auto ball_ptr = world.emplace_object(std::move(ball), std::move(ball_status));
    auto ball_2_ptr = world.emplace_object(std::move(ball_2), std::move(ball_2_status));

    world.time().start();

    print_loop(&world, {ball_ptr, ball_2_ptr});
}

void world_and_boom()
{
    World world;
    world.set_boundary(20, -10, 0, 50);

    std::vector<std::shared_ptr<Object>> objptr_vec;
    for (int i = 0; i != 3; ++i)
    {
        for (int j = 0; j != 3; ++j)
        {
            Object ball(std::to_string(i * 3 + j + 1), 1);
            ball.elasticity() = 0.9;
            ball.friction() = 0.01;
            ObjectStatus ball_status(Coordinate(10 + j * 5, i * 5 + 1), Velocity(0, 0), {Force(0, -9.8)});
            auto ptr = world.emplace_object(std::move(ball), std::move(ball_status));
            objptr_vec.push_back(ptr);
        }
    }

    world.time().timeflow() = 2.5;
    world.time().start();

    std::thread t(print_loop, &world, objptr_vec, 1.0);
    sleep(1);
    world.explode(Coordinate(16, 0.5), 10000);
    t.join();
}

void world_and_many_ball()
{
    World world;
    world.set_boundary(20, 0, 0, 50);

    std::vector<std::shared_ptr<Object>> objptr_vec;
    for (int i = 0; i != 10; ++i)
    {
        Object ball(std::to_string(i), 1);
        ball.elasticity() = 0.9;
        ball.friction() = 0.1;
        ObjectStatus ball_status(Coordinate(i * 2, i * 2), Velocity(i, 0), {Force(0, -9.8)});
        auto ptr = world.emplace_object(std::move(ball), std::move(ball_status));
        objptr_vec.push_back(ptr);
    }

    world.time().timeflow() = 2.5;
    world.time().start();

    print_loop(&world, objptr_vec);
}

void print_loop(Space *space, const std::vector<std::shared_ptr<Object>> &objects, double scale)
{
    printf("\033[?25l\033[2J");

    std::vector<std::pair<int, int>> pre_coor(objects.size());

    std::string print_str = "\033[%d;%dH \033[%d;%dHO";

    while (true)
    {
        printf("\033[?25l\033[2J");
        for (size_t i = 0; i != objects.size(); ++i)
        {
            auto &&[doublex, doubley] = space->object_status(objects[i]).coordinate() * scale;
            int x = static_cast<int>(std::round(doublex)) + 30;
            int y = -static_cast<int>(std::round(doubley)) + 20;

            auto &&[pre_x, pre_y] = pre_coor[i];

            printf("\033[%d;%dH%s\n", y, x, objects[i]->name().c_str());

            pre_x = x;
            pre_y = y;
        }
        usleep(1000 * 10);
    }
}