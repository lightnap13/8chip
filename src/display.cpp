#include "display.hpp"

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
