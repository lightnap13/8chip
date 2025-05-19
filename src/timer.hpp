#ifndef CHIP8_SRC_TIMERHPP
#define CHIP8_SRC_TIMERHPP

#include <cstdint>

namespace chip8
{
    class cTimer
    {
      public:
        enum class eType
        {
            undefined,
            delay,
            sound,
        };

        explicit cTimer(eType type);
        void    update();
        uint8_t get_time() const;
        void    set_time(uint8_t time);

      private:
        uint8_t _time {0U};
        eType   _type {eType::undefined};
    };

}

#endif // CHIP8_SRC_TIMERHPP
