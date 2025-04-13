#ifndef CHIP8_SRC_DISPLAYHPP
#define CHIP8_SRC_DISPLAYHPP

#include <assert.h>
#include <cstdint>
#include <vector>

namespace chip8
{
    constexpr int32_t DISPLAY_WIDTH = 64;
    constexpr int32_t DISPLAY_HEIGHT = 32;

    constexpr char EMPTY_PIXEL_CHAR = '.';
    constexpr char FULL_PIXEL_CHAR = '#';

    class cDisplay
    {
      public:
        cDisplay(int32_t height, int32_t width);

        void draw_frame();
        void clear_pixels();

      private:
        void clear_terminal();
        void print_pixels();

        int32_t              _height;
        int32_t              _width;
        std::vector<int32_t> _pixels;
    };
}

#endif // CHIP8_SRC_DISPLAYHPP
