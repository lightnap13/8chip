#include "timer.hpp"

namespace chip8
{

    cTimer::cTimer(cTimer::eType type)
      : _type(type)
    {
    }

    void cTimer::update()
    {
        // TODO: Make this update at 60Hz per second.
        if (_time > 0)
        {
            _time--;
        }

        if (_time <= 0 && _type == eType::sound)
        {
            // TODO: Make it so that sound timer beeps when reaching 0.
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
