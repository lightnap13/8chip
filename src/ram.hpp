#ifndef CHIP8_SRC_RAMHPP
#define CHIP8_SRC_RAMHPP

#include <cstdint>
#include <string>
#include <vector>

namespace chip8
{
    constexpr int32_t RAM_SIZE = 0xFFF;
    constexpr int32_t PROGRAM_START_LOCATION = 0x200;
    constexpr int32_t FONT_START_LOCATION = 0x50;
    constexpr int32_t FONT_SIZE = 5;

    class cRam
    {
      public:
        cRam(int32_t size, int32_t program_offset) noexcept;

        int32_t load_rom(std::string path) & noexcept;
        void    clear() & noexcept;
        void    print() const& noexcept;

        int32_t size() const& noexcept;
        uint8_t read(int32_t index) const& noexcept;
        void    write(int32_t index, uint8_t value) noexcept;

        void     push_to_stack(uint16_t value) & noexcept;
        uint16_t pop_from_stack() & noexcept;

        uint16_t get_font_char_position(uint8_t character) & noexcept;

      private:
        void init_font() & noexcept;

        int32_t               _program_offset;
        std::vector<uint8_t>  _ram;
        std::vector<uint16_t> _stack;
    };
}

#endif // CHIP8_SRC_RAMHPP
