#include <iostream>
#include <string>
#include <cmath>
#include <cstdarg>
#include <unistd.h>

#include "physics.h"

using namespace meophys;

void world_and_ball();
void interstellar_and_planet();
void print_loop(const Space *space, std::vector<std::shared_ptr<Object>> &&objects, double scale = 1.0);

int main()
{
    world_and_ball();
    // interstellar_and_planet();
    return 0;
}

void interstellar_and_planet()
{
    Interstellar space;

    //auto sun = std::make_shared<Object>("Sun", 1.989e30);
    auto earth = std::make_shared<Object>("Earth", 5.965e24);
    auto month = std::make_shared<Object>("Month", 7.349e22);
    month->set_velocity(0, 1.023e3);

    //space.emplace_object(sun, Coordinate(0, 0));
    space.emplace_object(earth, Coordinate(0, 0));
    space.emplace_object(month, Coordinate(384403.9e3, 0));

    space.time().timeflow() = 1000000;
    space.time().start();

    print_loop(&space, {earth, month}, 1e-8);
}

void world_and_ball()
{
    World world;
    world.set_boundary(11, -10, 0, 50);

    auto ball = std::make_shared<Object>("Ball", 1);
    ball->elasticity() = 0.9;
    ball->friction() = 0.5;
    ball->exert_force(Force(0, -9.8));
    ball->set_velocity(5, 0);

    auto ball_2 = std::make_shared<Object>("Ball2", 1);
    ball_2->elasticity() = 0.9;
    ball_2->friction() = 0.5;
    ball_2->exert_force(Force(0, -9.8));
    ball_2->set_velocity(-5, 0);

    world.emplace_object(ball, Coordinate(1, 10));
    world.emplace_object(ball_2, Coordinate(20, 10));

    world.time().start();

    print_loop(&world, {ball, ball_2});
}

void print_loop(const Space *space, std::vector<std::shared_ptr<Object>> &&objects, double scale)
{
    printf("\033[?25l\033[2J");

    std::vector<std::pair<int, int>> pre_coor(objects.size());

    std::string print_str = "\033[%d;%dH \033[%d;%dHO";

    while (true)
    {
        for (size_t i = 0; i != objects.size(); ++i)
        {
            auto &&[doublex, doubley] = space->get_coor(objects[i]) * scale;
            int x = static_cast<int>(std::round(doublex)) + 30;
            int y = -static_cast<int>(std::round(doubley / 2.0)) + 10;

            auto &&[pre_x, pre_y] = pre_coor[i];
            if (pre_x == x && pre_y == y)
            {
                usleep(10);
                continue;
            }
            printf("\033[%d;%dH \033[%d;%dHO\n", pre_y, pre_x, y, x);

            pre_x = x;
            pre_y = y;
        }
        usleep(10);
    }
}