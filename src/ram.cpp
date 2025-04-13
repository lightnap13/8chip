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

        std::cout << "[DEBUG] Ram has been sucessfully filled" << std::endl;
        return 0;
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

    void cRam::print()
    {
        int32_t lines_width = 16; // 16 bytes per line.

        log::log(log::eLevel::debug, "Printing ram");

        for (int y = 0; y < _ram.size() / lines_width; y++)
        {
            for (int x = 0; x < lines_width; x++)
            {
                uint8_t current_byte = _ram[y * lines_width + x];
                printf("%02x ", current_byte);
                if (x % 8 == 7)
                {
                    printf(" ");
                }
            }

            std::cout << "\n";
        }

        std::cout.flush();
    }
}
