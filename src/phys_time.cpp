#include "phys_time.h"

#include "space.h"

#include <iostream>

using namespace meophys;

Time::Time(CallbackFunc on_tick, Space *space_ptr)
    : _on_tick(on_tick),
      _space_ptr(space_ptr)
{
}

Time::~Time()
{
    std::unique_lock<std::mutex> lock(_start_mutex);
    _stop = true;
    if (_tick_thread.joinable())
    {
        _tick_thread.join();
    }
}

void Time::start()
{
    std::unique_lock<std::mutex> lock(_start_mutex);
    if (!_tick_thread.joinable())
    {
        _tick_thread = std::thread(tick, this, _space_ptr);
    }
    else
    {
        lock.unlock();
        std::cerr << "Tick thread already started" << std::endl;
    }
}

void Time::stop()
{
    std::unique_lock<std::mutex> lock(_start_mutex);
    _stop = true;
    if (_tick_thread.joinable())
    {
        _tick_thread.join();
    }
}

void Time::pause()
{
    std::unique_lock<std::mutex> lock(_pause_mutex);
    _pause = true;
    // _pause_cv.notify_one();
}

void Time::resume()
{
    {
        std::unique_lock<std::mutex> lock(_pause_mutex);
        _pause = false;
    }
    _pause_cv.notify_one();
}

void Time::tick(Time *p_this, Space *p_space)
{
    while (!p_this->_stop)
    {
        auto begin_time = std::chrono::system_clock::now();
        double tick_time = PlanckTime * p_this->_timeflow; // seconds
        p_this->_on_tick(p_space, tick_time);
        std::unique_lock<std::shared_mutex> _elapsed_lock(p_this->_elapsed_mutex);
        p_this->_elapsed += std::chrono::nanoseconds(static_cast<int64_t>(tick_time * std::giga::num));
        _elapsed_lock.unlock();
        {
            std::unique_lock<std::mutex> pause_lock(p_this->_pause_mutex);
            p_this->_pause_cv.wait(pause_lock, [&p_this] { return !p_this->_pause; });
        }
        std::this_thread::sleep_until(begin_time + std::chrono::nanoseconds(ProcessInterval));
    }
}