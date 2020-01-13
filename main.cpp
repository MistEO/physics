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
    world.push_boundary(Line(0, 0));

    Object ball(10, Coordinate(10, 10));
    ball.elasticity() = 0.85;
    world.push_object(&ball);

    ball.push_force(Force(0, -10));

    Time::get_instance().start();

    while (true) {
        int y = ball.coordinate().second;
        std::cout << std::string(10 - y, '\n') << "o" << std::endl;
        usleep(100);
        system("clear");
    }
    return 0;
}