#ifndef CHIP8_SRC_RAMHPP
#define CHIP8_SRC_RAMHPP

#include <cstdint>
#include <string>
#include <vector>

namespace chip8
{
    constexpr int32_t RAM_SIZE = 0xFFF;
    constexpr int32_t PROGRAM_START_LOCATION = 0x200;

    class cRam
    {
      public:
        cRam(int32_t size, int32_t program_offset);

        int32_t load_rom(std::string path);
        void    clear();
        void    print();

        int32_t size();
        uint8_t read(int32_t index);
        void    write(int32_t index, uint8_t value);

        void     push_to_stack(uint16_t value);
        uint16_t pop_from_stack();

      private:
        int32_t               _program_offset;
        std::vector<uint8_t>  _ram;
        std::vector<uint16_t> _stack;
    };
}

#endif // CHIP8_SRC_RAMHPP
