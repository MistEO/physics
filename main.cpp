#include <iostream>
#include <string>
#include <unistd.h>

#include "define.h"
#include "object.h"
#include "space.h"
#include "time.h"

int main()
{
    Space world;
    Time::get_instance().push_space(&world);
    world.push_boundary(Boundary(BoundaryX, 0));
    world.push_boundary(Boundary(BoundaryX, 20));
    world.push_boundary(Boundary(BoundaryY, 0));
    world.push_boundary(Boundary(BoundaryY, 20));

    Object ball(1, Coordinate(0, 0), Velocity(10, 0));
    ball.elasticity = 0.9;
    ball.friction = 0.1;
    world.push_object(&ball);

    ball.forces.push_back(Force(0, -9.8) * ball.mass);

    Time::get_instance().start();

    while (true) {
        int x = ball.coordinate.first;
        int y = ball.coordinate.second;
        std::cout << std::string(10 - y, '\n') << std::string(x, ' ') << "o" << std::endl;
        usleep(100);
        system("clear");
    }
    return 0;
}