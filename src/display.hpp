#ifndef CHIP8_SRC_DISPLAYHPP
#define CHIP8_SRC_DISPLAYHPP

#include <assert.h>
#include <cstdint>
#include <vector>

namespace chip8
{
    constexpr int32_t DISPLAY_WIDTH = 64;
    constexpr int32_t DISPLAY_HEIGHT = 32;

    constexpr char EMPTY_PIXEL_CHAR[] = " ";
    constexpr char FULL_PIXEL_CHAR[] = "\u2588";

    class cDisplay
    {
      public:
        cDisplay(int32_t height, int32_t width) noexcept;

        void draw_frame() & noexcept;
        void clear_frame() & noexcept;

        void draw_byte(uint8_t sprite_initial_x, uint8_t y, uint8_t byte, bool* flipped_bit) & noexcept;

        int32_t get_height() const& noexcept;
        int32_t get_width() const& noexcept;

      private:
        void clear_terminal() const& noexcept;
        void present_frame() const& noexcept;

        int32_t           _height;
        int32_t           _width;
        std::vector<bool> _pixels;
    };
}

#endif // CHIP8_SRC_DISPLAYHPP
