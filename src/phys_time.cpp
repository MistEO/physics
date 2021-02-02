#include "phys_time.h"

#include "space.h"

using namespace meophys;

Time::Time(std::function<void(Space *)> on_tick, Space *space_ptr)
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
    _starting = std::chrono::system_clock::now();
    _tick_thread = std::thread(tick, this, _space_ptr);
}

void Time::pause()
{
    // TODO
}

void Time::tick(Time *p_this, Space *p_space)
{
    while (!p_this->_tick_over)
    {
        p_this->_on_tick(p_space);
        std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
    }
}