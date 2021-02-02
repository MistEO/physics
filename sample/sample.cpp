#include <iostream>
#include <string>
#include <unistd.h>

#include "physics.h"

using namespace meophys;

void world_and_ball();
void interstellar_and_planet();

int main()
{
    interstellar_and_planet();
    return 0;
}

void interstellar_and_planet()
{
    Interstellar space;

    auto earth = std::make_shared<Object>(5.965e24);
    auto month = std::make_shared<Object>(7.349e22);

    space.emplace_object(earth, Coordinate(0, 0));
    space.emplace_object(month, Coordinate(384403.9e3, 0));

    printf("\033[?25l\033[2J");
    Coordinate pre_earth(0, 0);
    Coordinate pre_month(0, 0);

    space.time().start();

    while (true)
    {
        auto earth_coor = space.get_coor(earth) / 1e7;
        auto month_coor = space.get_coor(month) / 1e7;
        printf("\033[%d;%dH  \033[%d;%dHOe\n", int(pre_earth.second), int(pre_earth.first), int(earth_coor.second), int(earth_coor.first));
        printf("\033[%d;%dH  \033[%d;%dHOm\n", int(pre_month.second), int(pre_earth.first), int(month_coor.second), int(month_coor.first));
        pre_earth = earth_coor;
        pre_month = earth_coor;
    }
}

void world_and_ball()
{
    World world;
    world.set_boundary(10, 0, 0, 50);

    auto ball = std::make_shared<Object>(1, 0.9, 0.9);
    ball->exert_force(Force(0, -9.8));
    ball->velocity().first = 5;

    world.emplace_object(ball, Coordinate(0, 10));

    printf("\033[?25l\033[2J");
    int pre_x = 1, pre_y = 1;

    world.time().start();

    while (true)
    {
        auto coor = world.get_coor(ball) + Coordinate(1, 1);
        int x = (coor.first > 0.0) ? (coor.first + 0.5) : (coor.first - 0.5);
        int y = (coor.second > 0.0) ? (coor.second + 0.5) : (coor.second - 0.5);
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