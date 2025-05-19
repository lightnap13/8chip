#include "display.hpp"

#include <algorithm>
#include <iostream>

namespace chip8
{
    cDisplay::cDisplay(int32_t height, int32_t width)
    {
        _height = height;
        _width = width;
        _pixels.reserve(_height * _width);

        for (uint32_t i = 0U; i < _width * _height; i++)
        {
            _pixels.push_back(0);
        }

        assert(_pixels.size() == _width * _height);
    }

    void cDisplay::draw_frame()
    {
        clear_terminal();
        print_pixels();
    }

    void cDisplay::clear_pixels()
    {
        for (uint32_t i = 0U; i < _width * _height; i++)
        {
            _pixels[i] = 0;
        }
    }

    void cDisplay::draw_byte(uint8_t sprite_initial_x, uint8_t y, uint8_t byte, bool* flipped_bit)
    {
        // We don't draw past the edge of the screen.
        if (y >= _height)
        {
            return;
        }

        bool flipped_any_bit = false;

        // We don't draw past the edge of the screen.
        uint8_t sprite_final_x = std::min(_width, sprite_initial_x + 8);
        uint8_t sprite_width = sprite_final_x - sprite_initial_x;

        for (uint8_t i = 0; i < sprite_width; i++)
        {
            uint8_t current_sprite_pixel = (byte >> (7 - i)) & 0b1;
            size_t  current_display_location = y * _width + sprite_initial_x + i;
            uint8_t current_display_pixel = static_cast<uint8_t>(_pixels[current_display_location]);

            if (current_sprite_pixel && current_display_pixel)
            {
                flipped_any_bit = true;
                _pixels[current_display_location] = 0;
            }

            if (current_sprite_pixel && !current_display_pixel)
            {
                _pixels[current_display_location] = 1;
            }
        }

        *flipped_bit = flipped_any_bit;
    }

    int32_t cDisplay::get_height()
    {
        return _height;
    }

    int32_t cDisplay::get_width()
    {
        return _width;
    }

    void cDisplay::clear_terminal()
    {
        std::cout << "\e[1;1H\e[2J";
    }

    void cDisplay::print_pixels()
    {
        std::string ascii_display {};
        ascii_display.reserve(_height * _width + _height);

        for (int y = 0; y < _height; y++)
        {
            for (int x = 0; x < _width; x++)
            {
                int current_pixel = _pixels[y * _width + x];
                ascii_display += current_pixel == 0 ? EMPTY_PIXEL_CHAR : FULL_PIXEL_CHAR;
            }

            ascii_display += '\n';
        }

        std::cout << ascii_display;
    }
}
