#pragma once

#include "object.h"
#include "space.h"
#include <mutex>
#include <vector>

class Time {
public:
    static Time& get_instance();
    void push_space(Space* space);
    void set_proportion(double value);
    void start();
    int ticks() const;

private:
    Time() = default;

    void tick(double time = 1.0);

    std::vector<Space*> spaces;
    std::mutex tick_mutex;
    std::mutex time_mutex;
    double proportion = 1.0; // 现实时间与之比例
    int ticks_count = 0;
};