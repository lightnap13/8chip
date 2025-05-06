#ifndef CHIP8_SRC_PROCESSORHPP
#define CHIP8_SRC_PROCESSORHPP

#include <cstdint>
#include <vector>

namespace chip8
{
    constexpr int32_t REGISTER_COUNT = 16;

    class cDisplay;
    class cKeyboard;
    class cRam;

    class cProcessor
    {
      public:
        cProcessor(int32_t program_start_location, int32_t register_count);

        void execute_next_instruction(cRam* ram, cDisplay* display, cKeyboard* keyboard);

      private:
        void execute_opcode_0XXX(int16_t opcode, cDisplay* display);
        void execute_opcode_0E00(int16_t opcode, cDisplay* display);
        void execute_opcode_00EE(int16_t opcode);
        void execute_opcode_1NNN(int16_t opcode);
        void execute_opcode_2NNN(int16_t opcode);
        void execute_opcode_3XNN(int16_t opcode);
        void execute_opcode_4XNN(int16_t opcode);
        void execute_opcode_5XY0(int16_t opcode);
        void execute_opcode_6XNN(int16_t opcode);
        void execute_opcode_7XNN(int16_t opcode);
        void execute_opcode_8XXX(int16_t opcode);
        void execute_opcode_8XY0(int16_t opcode);
        void execute_opcode_8XY1(int16_t opcode);
        void execute_opcode_8XY2(int16_t opcode);
        void execute_opcode_8XY3(int16_t opcode);
        void execute_opcode_8XY4(int16_t opcode);
        void execute_opcode_8XY5(int16_t opcode);
        void execute_opcode_8XY6(int16_t opcode);
        void execute_opcode_8XY7(int16_t opcode);
        void execute_opcode_8XYE(int16_t opcode);
        void execute_opcode_9XY0(int16_t opcode);
        void execute_opcode_ANNN(int16_t opcode);
        void execute_opcode_BNNN(int16_t opcode);
        void execute_opcode_CXNN(int16_t opcode);
        void execute_opcode_DXYN(int16_t opcode, cRam* ram, cDisplay* display);
        void execute_opcode_EXXX(int16_t opcode, cKeyboard* keyboard);
        void execute_opcode_EX9E(int16_t opcode, cKeyboard* keyboard);
        void execute_opcode_EXA1(int16_t opcode, cKeyboard* keyboard);
        void execute_opcode_FXXX(int16_t opcode, cRam* ram, cKeyboard* keyboard); // Needs sound and delay timers.
        void execute_opcode_FX07(int16_t opcode);                                 // Needs delay timer
        void execute_opcode_FX0A(int16_t opcode, cKeyboard* keyboard);
        void execute_opcode_FX15(int16_t opcode); // Needs delay timer
        void execute_opcode_FX18(int16_t opcode); // Needs sound timer
        void execute_opcode_FX1E(int16_t opcode);
        void execute_opcode_FX29(int16_t opcode);
        void execute_opcode_FX33(int16_t opcode);
        void execute_opcode_FX55(int16_t opcode, cRam* ram);
        void execute_opcode_FX65(int16_t opcode, cRam* ram);

        int16_t              _program_counter;
        int16_t              _register_i;
        std::vector<uint8_t> _registers;
    };
}

#endif // CHIP8_SRC_PROCESSORHPP
