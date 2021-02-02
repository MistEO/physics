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
        static constexpr int SleepTime = PlanckTime * 1e9; // nanoseconds
        using CallbackFunc = std::function<void(Space *, double)>;

    public:
        Time(CallbackFunc on_tick, Space *space_ptr);
        ~Time();

        void start();
        void pause();

    private:
        static void tick(Time *p_this, Space *p_space);
        std::chrono::system_clock::time_point _starting;
        CallbackFunc _on_tick = nullptr;
        std::thread _tick_thread;
        std::atomic<bool> _tick_over = false;
        Space *_space_ptr;
    };
} // namespace meophys