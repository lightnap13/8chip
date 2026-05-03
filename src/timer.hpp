#ifndef CHIP8_SRC_TIMERHPP
#define CHIP8_SRC_TIMERHPP

#include <chrono>
#include <cstdint>

namespace chip8
{
    class cTimer
    {
      public:
        explicit cTimer(double period);
        uint8_t get_time() const;
        void    set_time(uint8_t time);
        void    print();

      private:
        std::chrono::time_point<std::chrono::system_clock> _clock_start_time {};
        std::chrono::duration<double, std::milli>          _period {0.0};

        uint8_t _cycles {0U};
    };

}

#endif // CHIP8_SRC_TIMERHPP
