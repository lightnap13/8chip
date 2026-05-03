#include "timer.hpp"
#include "log.hpp"

#include <algorithm>
#include <chrono>

namespace chip8
{
    cTimer::cTimer(double period)
      : _period(period)
    {
    }

    uint8_t cTimer::get_time() const
    {
        auto ms_since_clock_start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) -
                                    std::chrono::duration_cast<std::chrono::milliseconds>(_clock_start_time.time_since_epoch());
        int periods_remaining = _cycles - ms_since_clock_start / _period;

        return std::max(0, periods_remaining);
    }

    void cTimer::set_time(uint8_t time)
    {
        _clock_start_time = std::chrono::system_clock::now();
        _cycles = time;
    }

    void cTimer::print()
    {
        thoth::debug("Printing timer information.\nPeriod: %d\nTime of last set_time: %d\nCountdown start value: %d\nCountdown current value: %d",
                     _period.count(),
                     _clock_start_time.time_since_epoch().count(),
                     static_cast<int>(_cycles),
                     static_cast<int>(get_time()));
    }
}
