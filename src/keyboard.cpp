#include "keyboard.hpp"

namespace chip8
{
    bool cKeyboard::is_key_pressed(uint8_t key_id)
    {
        // TODO: Proper keyboard input.
        return false;
    }

    int8_t cKeyboard::await_key_press()
    {
        // TODO: Proper keyboard input.
        // TODO: Return -1 when key not pressed. failure.
        // TODO: Assert that return value is <=0xF.
        return -1;
    }
}
