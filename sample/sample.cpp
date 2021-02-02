#include <iostream>
#include <string>
#include <unistd.h>

#include "physics.h"

int main()
{
    using namespace meophys;

    World world;
    world.set_boundary(10, 0, 0, 10);

    auto ball = std::make_shared<Object>(1, 0.9);
    ball->exert_force(Force(0, -9.8));

    world.emplace_object(ball, Coordinate(0, 10));

    printf("\033[?25l\033[2J");
    int pre_x = 1, pre_y = 1;

    world.time().start();

    while (true)
    {
        auto coor = world.object_coordinate(ball) + Coordinate(1, 1);
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
    return 0;
}