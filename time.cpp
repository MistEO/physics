#include "time.h"

#include <iostream>
#include <thread>
#include <unistd.h>

Time& Time::get_instance()
{
    static Time unique_instance;
    return unique_instance;
}

void Time::tick(double time)
{
    std::lock_guard<std::mutex> lock(tick_mutex);

    for (const auto& space : spaces) {
        space->tick(time);
    }
}

void Time::push_space(Space* space)
{
    spaces.push_back(space);
}

void Time::set_proportion(double value)
{
    proportion = value;
}

void Time::start()
{
    if (!time_mutex.try_lock()) {
        std::cerr << "The time has started" << std::endl;
        return;
    }
    std::thread tick_thread([&]() {
        while (true) {
            this->tick(proportion * 0.001);
            usleep(proportion * 1000);
        }
    });
    tick_thread.detach();
}