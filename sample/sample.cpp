#include <iostream>
#include <string>
#include <unistd.h>

#include "physics.h"

int main()
{
    using namespace meophys;

    World world;

    // World world;
    // Time::get_instance().push_space(&world);
    // world.set_boundary(Coordinate(0, 0), Coordinate(30, 10));
    // world.set_gravity(-9.8);

    // Object ball(100, Coordinate(0, 10), Velocity(10, 0));
    // world.push_object(&ball);
    // ball.elasticity = 0.5;
    // ball.friction = 0.1;

    // printf("\033[?25l\033[2J");
    // Time::get_instance().start();

    // int pre_x = 1, pre_y = 1;
    // while (true)
    // {
    //     Coordinate coord = ball.coordinate + Coordinate(1, 1);
    //     int x = (coord.first > 0.0) ? (coord.first + 0.5) : (coord.first - 0.5);
    //     int y = (coord.second > 0.0) ? (coord.second + 0.5) : (coord.second - 0.5);
    //     y = 11 - y;
    //     if (pre_x == x && pre_y == y)
    //     {
    //         usleep(10);
    //         continue;
    //     }
    //     printf("\033[%d;%dH \033[%d;%dHO\n", pre_y, pre_x, y, x);
    //     pre_x = x;
    //     pre_y = y;
    //     usleep(10);
    // }
    return 0;
}