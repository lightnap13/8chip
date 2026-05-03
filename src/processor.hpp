#ifndef CHIP8_SRC_PROCESSORHPP
#define CHIP8_SRC_PROCESSORHPP

#include <cstdint>
#include <vector>

namespace chip8
{
    constexpr int32_t REGISTER_COUNT = 16;
    constexpr int32_t INSTR_PER_SEC = 700;

    class cDisplay;
    class cKeyboard;
    class cRam;
    class cTimer;

    class cProcessor
    {
      public:
        cProcessor(int32_t program_start_location, int32_t register_count) noexcept;

        void execute_next_instruction(cRam* ram, cDisplay* display, cKeyboard* keyboard, cTimer* delay_timer, cTimer* sound_timer) & noexcept;

      private:
        void execute_opcode_0XXX(uint16_t opcode, cRam* ram, cDisplay* display) & noexcept;
        void execute_opcode_00E0(uint16_t opcode, cDisplay* display) & noexcept;
        void execute_opcode_00EE(uint16_t opcode, cRam* ram) & noexcept;
        void execute_opcode_1NNN(uint16_t opcode) & noexcept;
        void execute_opcode_2NNN(uint16_t opcode, cRam* ram) & noexcept;
        void execute_opcode_3XNN(uint16_t opcode) & noexcept;
        void execute_opcode_4XNN(uint16_t opcode) & noexcept;
        void execute_opcode_5XY0(uint16_t opcode) & noexcept;
        void execute_opcode_6XNN(uint16_t opcode) & noexcept;
        void execute_opcode_7XNN(uint16_t opcode) & noexcept;
        void execute_opcode_8XXX(uint16_t opcode) & noexcept;
        void execute_opcode_8XY0(uint16_t opcode) & noexcept;
        void execute_opcode_8XY1(uint16_t opcode) & noexcept;
        void execute_opcode_8XY2(uint16_t opcode) & noexcept;
        void execute_opcode_8XY3(uint16_t opcode) & noexcept;
        void execute_opcode_8XY4(uint16_t opcode) & noexcept;
        void execute_opcode_8XY5(uint16_t opcode) & noexcept;
        void execute_opcode_8XY6(uint16_t opcode) & noexcept;
        void execute_opcode_8XY7(uint16_t opcode) & noexcept;
        void execute_opcode_8XYE(uint16_t opcode) & noexcept;
        void execute_opcode_9XY0(uint16_t opcode) & noexcept;
        void execute_opcode_ANNN(uint16_t opcode) & noexcept;
        void execute_opcode_BNNN(uint16_t opcode) & noexcept;
        void execute_opcode_CXNN(uint16_t opcode) & noexcept;
        void execute_opcode_DXYN(uint16_t opcode, cRam* ram, cDisplay* display) & noexcept;
        void execute_opcode_EXXX(uint16_t opcode, cKeyboard* keyboard) & noexcept;
        void execute_opcode_EX9E(uint16_t opcode, cKeyboard* keyboard) & noexcept;
        void execute_opcode_EXA1(uint16_t opcode, cKeyboard* keyboard) & noexcept;
        void execute_opcode_FXXX(uint16_t opcode, cRam* ram, cKeyboard* keyboard, cTimer* delay_timer, cTimer* sound_timer) & noexcept;
        void execute_opcode_FX07(uint16_t opcode, cTimer* delay_timer) & noexcept;
        void execute_opcode_FX0A(uint16_t opcode, cKeyboard* keyboard) & noexcept;
        void execute_opcode_FX15(uint16_t opcode, cTimer* delay_timer) & noexcept;
        void execute_opcode_FX18(uint16_t opcode, cTimer* sound_timer) & noexcept;
        void execute_opcode_FX1E(uint16_t opcode) & noexcept;
        void execute_opcode_FX29(uint16_t opcode, cRam* ram) & noexcept;
        void execute_opcode_FX33(uint16_t opcode, cRam* ram) & noexcept;
        void execute_opcode_FX55(uint16_t opcode, cRam* ram) & noexcept;
        void execute_opcode_FX65(uint16_t opcode, cRam* ram) & noexcept;

        uint16_t             _program_counter;
        uint16_t             _register_i;
        std::vector<uint8_t> _registers;
    };
}

#endif // CHIP8_SRC_PROCESSORHPP
