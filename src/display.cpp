#include "display.hpp"

#include <algorithm>
#include <iostream>

namespace chip8
{
    cDisplay::cDisplay(int32_t height, int32_t width) noexcept
      : _height(height)
      , _width(width)
      , _pixels(_height * _width, false)
    {
    }

    void cDisplay::draw_frame() & noexcept
    {
        clear_terminal();
        present_frame();
    }

    void cDisplay::clear_frame() & noexcept
    {
        for (uint32_t i = 0U; i < _width * _height; i++)
        {
            _pixels[i] = false;
        }
    }

    void cDisplay::draw_byte(uint8_t sprite_initial_x, uint8_t y, uint8_t byte, bool* flipped_bit) & noexcept
    {
        // We don't draw past the edge of the screen.
        if (y >= _height)
        {
            return;
        }

        bool flipped_any_bit = false;

        // We don't draw past the edge of the screen.
        uint8_t const sprite_final_x = std::min(_width, sprite_initial_x + 8);
        uint8_t const sprite_width = sprite_final_x - sprite_initial_x;

        for (uint8_t i = 0; i < sprite_width; i++)
        {
            size_t const pixel_index = y * _width + sprite_initial_x + i;
            bool const   old_pixel_value = (byte >> (7 - i) & 0b1) == 1;
            bool const   new_pixel_value = _pixels[pixel_index];

            flipped_any_bit |= new_pixel_value && old_pixel_value;
            _pixels[pixel_index] = new_pixel_value != old_pixel_value;
        }

        *flipped_bit = flipped_any_bit;
    }

    int32_t cDisplay::get_height() const& noexcept
    {
        return _height;
    }

    int32_t cDisplay::get_width() const& noexcept
    {
        return _width;
    }

    void cDisplay::clear_terminal() const& noexcept
    {
        std::cout << "\e[1;1H\e[2J";
    }

    void cDisplay::present_frame() const& noexcept
    {
        std::string ascii_display {};
        ascii_display.reserve((std::max(sizeof(EMPTY_PIXEL_CHAR), sizeof(FULL_PIXEL_CHAR)) - 1) * (_height * _width + _height));

        for (int y = 0; y < _height; y++)
        {
            for (int x = 0; x < _width; x++)
            {
                ascii_display += _pixels[y * _width + x] ? FULL_PIXEL_CHAR : EMPTY_PIXEL_CHAR;
            }

            ascii_display += '\n';
        }

        std::cout << ascii_display;
    }
}
