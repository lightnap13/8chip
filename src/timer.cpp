#include "timer.hpp"

namespace chip8
{
    void cTimer::update()
    {
        // TODO: Make this update at 60Hz per second.
        // TODO: Make it so that sound timer beeps when reaching 0.
        if (_time > 0)
        {
            _time--;
        }
    }

    uint8_t cTimer::get_time() const
    {
        return _time;
    }

    void cTimer::set_time(uint8_t time)
    {
        _time = time;
    }
}
