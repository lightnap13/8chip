#ifndef CHIP8_SRC_KEYBOARDHPP
#define CHIP8_SRC_KEYBOARDHPP

#include <cstdint>

namespace chip8
{
    class cKeyboard
    {
      public:
        bool   is_key_pressed(uint8_t key_id) const& noexcept;
        int8_t await_key_press() const& noexcept;
    };
}
#endif // CHIP8_SRC_KEYBOARDHPP
