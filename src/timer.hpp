#ifndef CHIP8_SRC_TIMERHPP
#define CHIP8_SRC_TIMERHPP

#include <chrono>
#include <cstdint>

namespace chip8
{
    class cTimer
    {
      public:
        explicit cTimer(double period) noexcept;
        uint8_t get_time() const& noexcept;
        void    set_time(uint8_t time) & noexcept;
        void    print() const& noexcept;

      private:
        std::chrono::time_point<std::chrono::system_clock> _clock_start_time {};
        std::chrono::duration<double, std::milli>          _period {0.0};

        uint8_t _cycles {0U};
    };

}

#endif // CHIP8_SRC_TIMERHPP
