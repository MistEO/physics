#include "phys_time.h"

#include "space.h"

using namespace meophys;

Time::Time(CallbackFunc on_tick, Space *space_ptr)
    : _on_tick(on_tick),
      _space_ptr(space_ptr)
{
}

Time::~Time()
{
    _tick_over = true;
    if (_tick_thread.joinable())
    {
        _tick_thread.join();
    }
}

void Time::start()
{
    _tick_thread = std::thread(tick, this, _space_ptr);
}

void Time::pause()
{
    // TODO
}

void Time::tick(Time *p_this, Space *p_space)
{
    p_this->_starting = std::chrono::system_clock::now();
    while (!p_this->_tick_over)
    {
        auto start_time = std::chrono::system_clock::now();
        auto tick_time = PlanckTime * p_this->_timeflow; // seconds
        p_this->_on_tick(p_space, tick_time);
        p_this->_elapsed += std::chrono::nanoseconds(static_cast<int64_t>(tick_time * 1e9));
        std::this_thread::sleep_until(start_time + std::chrono::nanoseconds(ProcessInterval));
    }
}