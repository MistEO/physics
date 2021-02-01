#pragma once

#include <chrono>
#include <functional>
#include <thread>
#include <atomic>
#include "define.h"

namespace meophys
{
    class Space;

    class Time
    {

    public:
        Time(std::function<void(Space *)> callback_when_ticktime, Space *space_ptr);
        ~Time();

        void start();

    private:
        static void tick(Time *p_this, Space *p_space);
        std::chrono::system_clock::time_point _starting;
        std::function<void(Space *)> _callback_when_ticktime = nullptr;
        std::thread _tick_thread;
        std::atomic<bool> _tick_over = false;
    };
} // namespace meophys