#include "phys_time.h"

#include "space.h"

using namespace meophys;

Time::Time(std::function<void(Space *)> callback_when_ticktime, Space *space_ptr)
    : _callback_when_ticktime(callback_when_ticktime),
      _tick_thread(tick, this, space_ptr)
{
}

void Time::start()
{
    _starting = std::chrono::system_clock::now();
}

void Time::tick(Time *p_this, Space *p_space)
{
    while (true)
    {
        p_this->_callback_when_ticktime(p_space);
        std::this_thread::sleep_for(std::chrono::nanoseconds(PlankTime));
    }
}