#include <iostream>
#include <string>
#include <cmath>
#include <unistd.h>

#include "physics.h"

using namespace meophys;

void world_and_ball();
void interstellar_and_planet();

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
    month->set_velocity(Velocity(0, 1.023e3));

    //space.emplace_object(sun, Coordinate(0, 0));
    space.emplace_object(earth, Coordinate(0, 0));
    space.emplace_object(month, Coordinate(384403.9e3, 0));

    printf("\033[?25l\033[2J");
    int earth_pre_x = 0, earth_pre_y = 0, month_pre_x = 0, month_pre_y = 0;

    space.time().timeflow() = 1000000;
    space.time().start();

    while (true)
    {
        auto &&[earth_doublex, earth_doubley] = Coordinate(20, 20);
        auto &&[month_doublex, month_doubley] = 2 * (space.get_coor(month) - space.get_coor(earth)) / 1e8 + Coordinate(20, 20);

        int earth_x = static_cast<int>(std::round(earth_doublex));
        int earth_y = static_cast<int>(std::round(earth_doubley / 2));
        int month_x = static_cast<int>(std::round(month_doublex));
        int month_y = static_cast<int>(std::round(month_doubley / 2));

        auto elapsed = space.time().elapsed<std::ratio<3600 * 24>>().count();

        if (earth_x == earth_pre_x && earth_y == earth_pre_y &&
            month_x == month_pre_x && month_pre_y == earth_pre_y)
        {
            usleep(10);
            continue;
        }

        printf(
            "\033[0;0Helapsed: %ld days\033[%d;%dH \033[%d;%dHO\033[%d;%dH \033[%d;%dHO\n",
            elapsed,
            earth_pre_y,
            earth_pre_x, earth_y, earth_x,
            month_pre_y, month_pre_x, month_y, month_x);

        earth_pre_x = earth_x;
        earth_pre_y = earth_y;
        month_pre_x = month_x;
        month_pre_y = month_y;

        usleep(10);
    }
}

void world_and_ball()
{
    World world;
    world.set_boundary(11, -10, 0, 50);

    auto ball = std::make_shared<Object>("Ball", 1);
    ball->elasticity() = 0.9;
    ball->friction() = 0.5;
    ball->exert_force(Force(0, -9.8));
    ball->set_velocity(Velocity(5, 0));

    world.emplace_object(ball, Coordinate(1, 10));

    printf("\033[?25l\033[2J");
    int pre_x = 0, pre_y = 0;

    world.time().start();

    while (true)
    {
        auto &&[doublex, doubley] = world.get_coor(ball) + Coordinate(1, 1);
        int x = static_cast<int>(std::round(doublex));
        int y = static_cast<int>(std::round(doubley));
        y = 11 - y;
        if (pre_x == x && pre_y == y)
        {
            usleep(10);
            continue;
        }
        printf("\033[%d;%dH \033[%d;%dHO\n", pre_y, pre_x, y, x);
        pre_x = x;
        pre_y = y;
        usleep(10);
    }
}