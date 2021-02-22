#pragma once

#include <chrono>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include "define.h"

namespace meophys
{
    class Space;

    class Time
    {
        static constexpr double PlanckTime = 1e-3;  // seconds
        static constexpr int ProcessInterval = 1e6; // nanoseconds
        using CallbackFunc = std::function<void(Space *, double)>;

    public:
        Time(CallbackFunc on_tick, Space *space_ptr);
        ~Time();

        void start();
        void stop();
        void pause();
        void resume();

        bool is_running() const noexcept { return !_pause && _tick_thread.joinable(); }
        std::atomic<double> &timeflow() noexcept { return _timeflow; }

        template <typename Period>
        std::chrono::duration<int64_t, Period> elapsed() const
        {
#ifdef WIN32
            static_assert(std::_Is_ratio_v<Period>,
                          "period must be a specialization of ratio");
#else
            static_assert(std::chrono::__is_ratio<Period>::value,
                          "period must be a specialization of ratio");
#endif
            static_assert(Period::num > 0, "period must be positive");

            std::shared_lock<std::shared_mutex> lock(_elapsed_mutex);
            return std::chrono::duration_cast<std::chrono::duration<int64_t, Period>>(_elapsed);
        }

    private:
        static void tick(Time *p_this, Space *p_space);
        CallbackFunc _on_tick = nullptr;
        Space *_space_ptr = nullptr;
        std::atomic<double> _timeflow = 1.0;

        std::chrono::nanoseconds _elapsed;
        mutable std::shared_mutex _elapsed_mutex;
        std::thread _tick_thread;
        mutable std::mutex _start_mutex;
        std::atomic<bool> _stop = false;
        mutable std::mutex _pause_mutex;
        mutable std::condition_variable _pause_cv;
        std::atomic<bool> _pause = false;
    };
} // namespace meophys