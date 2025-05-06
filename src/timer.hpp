#ifndef CHIP8_SRC_TIMERHPP
#define CHIP8_SRC_TIMERHPP

#include <cstdint>

namespace chip8
{
    class cTimer
    {
      public:
        void    update();
        uint8_t get_time() const;
        void    set_time(uint8_t time);

      private:
        uint8_t _time {0U};
    };

}

#endif // CHIP8_SRC_TIMERHPP
