#include "ram.hpp"
#include "log.hpp"

#include <assert.h>

#include <fstream>
#include <iostream>

namespace chip8
{
    cRam::cRam(int32_t size, int32_t program_offset) noexcept
      : _program_offset(program_offset)
      , _ram(size, 0U)
      , _stack()
    {
    }

    void cRam::clear() & noexcept
    {
        for (int32_t i = 0; i < _ram.size(); i++)
        {
            _ram[i] = 0;
        }
    }

    int32_t cRam::load_rom(std::string path) & noexcept
    {
        thoth::info("Loading rom %s", path.c_str());

        std::ifstream file {path, std::ios::binary | std::ios::in};
        if (!file.is_open())
        {
            thoth::error("Could not load file %s", path.c_str());
            return -1;
        }

        thoth::debug("ROM was loaded correctly");

        uint64_t const max_program_size {_ram.size() - _program_offset};

        file.read(reinterpret_cast<char*>(&_ram[_program_offset]), max_program_size);

        if (file.gcount() >= max_program_size)
        {
            thoth::warning("File has completely filed ram. Might indicate ROM size too big");
        }

        init_font();

        thoth::debug("Ram has been sucessfully initialised");
        return 0;
    }

    void cRam::init_font() & noexcept
    {
        size_t index {FONT_START_LOCATION};

        auto const push_nibble_to_ram = [&](const uint8_t nibble) noexcept { _ram[index++] = nibble << 4; };

        // 0.
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1111);

        // 1.
        push_nibble_to_ram(0b0010);
        push_nibble_to_ram(0b0110);
        push_nibble_to_ram(0b0010);
        push_nibble_to_ram(0b0010);
        push_nibble_to_ram(0b0111);

        // 2.
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b0001);
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b1000);
        push_nibble_to_ram(0b1111);

        // 3.
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b0001);
        push_nibble_to_ram(0b0111);
        push_nibble_to_ram(0b0001);
        push_nibble_to_ram(0b1111);

        // 4.
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b0001);
        push_nibble_to_ram(0b0001);

        // 5.
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b1000);
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b0001);
        push_nibble_to_ram(0b1111);

        // 6.
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b1000);
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1111);

        // 7.
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b0001);
        push_nibble_to_ram(0b0001);
        push_nibble_to_ram(0b0001);
        push_nibble_to_ram(0b0001);

        // 8.
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1111);

        // 9.
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b0001);
        push_nibble_to_ram(0b0001);

        // A.
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1001);

        // B.
        push_nibble_to_ram(0b1110);
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1110);
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1110);

        // C.
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b1000);
        push_nibble_to_ram(0b1000);
        push_nibble_to_ram(0b1000);
        push_nibble_to_ram(0b1111);

        // D.
        push_nibble_to_ram(0b1110);
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1001);
        push_nibble_to_ram(0b1110);

        // E.
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b1000);
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b1000);
        push_nibble_to_ram(0b1111);

        // F.
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b1000);
        push_nibble_to_ram(0b1111);
        push_nibble_to_ram(0b1000);
        push_nibble_to_ram(0b1000);
    }

    int32_t cRam::size() const& noexcept
    {
        return _ram.size();
    }

    uint8_t cRam::read(int32_t index) const& noexcept
    {
        assert(index < _ram.size());
        // std::printf("[TRACE] Reading ram in position %d result %02x\n", index, _ram[index]);
        return _ram[index];
    }

    void cRam::write(int32_t index, uint8_t value) noexcept
    {
        assert(index < _ram.size());
        _ram[index] = value;
    }

    void cRam::push_to_stack(uint16_t value) & noexcept
    {
        _stack.push_back(value);
    }

    uint16_t cRam::pop_from_stack() & noexcept
    {
        assert(_stack.size() > 0);
        uint16_t const value = _stack.back();
        _stack.pop_back();
        return value;
    }

    uint16_t cRam::get_font_char_position(uint8_t character) & noexcept
    {
        assert(character <= 0xF);
        uint32_t const position = FONT_START_LOCATION + character * FONT_SIZE;
        assert(position < _ram.size());
        return static_cast<uint16_t>(position);
    }

    void cRam::print() const& noexcept
    {
        int32_t const lines_width = 16; //  bytes per line.
        int32_t const number_segments = 2;
        int32_t const bytes_per_segment = lines_width / number_segments;

        thoth::debug("Printing ram\n");

        for (int y = 0; y < _ram.size() / lines_width; y++)
        {
            for (int x = 0; x < lines_width; x++)
            {
                const uint8_t current_byte = _ram[y * lines_width + x];
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
