#include "timer.hpp"

#include <algorithm>
#include <chrono>
#include <iostream> // TODO: remove when logging works

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
        std::cout << "[INFO_] Printing timer information.\nPeriod: " << _period.count() << "\nTime of last set_time:" << _clock_start_time.time_since_epoch().count()
                  << "\nCountdown start value: " << static_cast<int>(_cycles) << "\nCountdown current value: " << static_cast<int>(get_time()) << '\n';
    }
}
