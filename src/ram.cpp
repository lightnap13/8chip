#include "ram.hpp"
#include "log.hpp"

#include <assert.h>

#include <fstream>
#include <iostream>

namespace chip8
{
    cRam::cRam(int32_t size, int32_t program_offset)
    {
        _stack.clear();
        _program_offset = program_offset;
        _ram = std::vector<uint8_t>(size, 0);
    }

    void cRam::clear()
    {
        for (int32_t i = 0; i < _ram.size(); i++)
        {
            _ram[i] = 0;
        }
    }

    int32_t cRam::load_rom(std::string path)
    {
        std::cout << "[ INFO] Loading rom " << path << "\n";

        std::ifstream file {path, std::ios::binary | std::ios::in};
        if (!file.is_open())
        {
            return -1;
        }

        std::cout << "[ INFO] ROM was loaded correctly" << std::endl;

        uint64_t max_program_size {_ram.size() - _program_offset};

        file.read(reinterpret_cast<char*>(&_ram[_program_offset]), max_program_size);

        std::streamsize bytes_read = file.gcount();
        if (bytes_read >= max_program_size)
        {
            std::cout << "[WARNING] File has completely filed ram. Might indicate ROM size too big" << std::endl;
        }

        init_font();

        std::cout << "[DEBUG] Ram has been sucessfully filled" << std::endl;
        return 0;
    }

    void cRam::init_font()
    {
        size_t index {FONT_START_LOCATION};

        // 0.
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1111 << 4;

        // 1.
        _ram[index++] = 0b0010 << 4;
        _ram[index++] = 0b0110 << 4;
        _ram[index++] = 0b0010 << 4;
        _ram[index++] = 0b0010 << 4;
        _ram[index++] = 0b0111 << 4;

        // 2.
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b0001 << 4;
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b1000 << 4;
        _ram[index++] = 0b1111 << 4;

        // 3.
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b0001 << 4;
        _ram[index++] = 0b0111 << 4;
        _ram[index++] = 0b0001 << 4;
        _ram[index++] = 0b1111 << 4;

        // 4.
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b0001 << 4;
        _ram[index++] = 0b0001 << 4;

        // 5.
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b1000 << 4;
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b0001 << 4;
        _ram[index++] = 0b1111 << 4;

        // 6.
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b1000 << 4;
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1111 << 4;

        // 7.
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b0001 << 4;
        _ram[index++] = 0b0001 << 4;
        _ram[index++] = 0b0001 << 4;
        _ram[index++] = 0b0001 << 4;

        // 8.
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1111 << 4;

        // 9.
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b0001 << 4;
        _ram[index++] = 0b0001 << 4;

        // A.
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1001 << 4;

        // B.
        _ram[index++] = 0b1110 << 4;
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1110 << 4;
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1110 << 4;

        // C.
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b1000 << 4;
        _ram[index++] = 0b1000 << 4;
        _ram[index++] = 0b1000 << 4;
        _ram[index++] = 0b1111 << 4;

        // D.
        _ram[index++] = 0b1110 << 4;
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1001 << 4;
        _ram[index++] = 0b1110 << 4;

        // E.
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b1000 << 4;
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b1000 << 4;
        _ram[index++] = 0b1111 << 4;

        // F.
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b1000 << 4;
        _ram[index++] = 0b1111 << 4;
        _ram[index++] = 0b1000 << 4;
        _ram[index++] = 0b1000 << 4;
    }

    int32_t cRam::size()
    {
        return _ram.size();
    }

    uint8_t cRam::read(int32_t index)
    {
        assert(index < _ram.size());
        // std::printf("[TRACE] Reading ram in position %d result %02x\n", index, _ram[index]);
        return _ram[index];
    }

    void cRam::write(int32_t index, uint8_t value)
    {
        assert(index < _ram.size());
        _ram[index] = value;
    }

    void cRam::push_to_stack(uint16_t value)
    {
        _stack.push_back(value);
    }

    uint16_t cRam::pop_from_stack()
    {
        assert(_stack.size() > 0);
        uint16_t value = _stack.back();
        _stack.pop_back();
        return value;
    }

    uint16_t cRam::get_font_char_position(uint8_t character)
    {
        assert(character <= 0xF);
        uint32_t position = FONT_START_LOCATION + character * FONT_SIZE;
        assert(position < _ram.size());
        return static_cast<uint16_t>(position);
    }

    void cRam::print()
    {
        int32_t lines_width = 16; //  bytes per line.
        int32_t number_segments = 2;
        int32_t bytes_per_segment = lines_width / number_segments;

        thoth::debug("Printing ram\n");

        for (int y = 0; y < _ram.size() / lines_width; y++)
        {
            for (int x = 0; x < lines_width; x++)
            {
                uint8_t current_byte = _ram[y * lines_width + x];
                printf("%02x ", current_byte);
                if ((x + 1) % bytes_per_segment == 0)
                {
                    printf(" ");
                }
            }

            std::cout << "\n";
        }

        std::cout.flush();
    }
}
