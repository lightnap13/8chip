#ifndef CHIP8_SRC_PROCESSORHPP
#define CHIP8_SRC_PROCESSORHPP

#include <cstdint>
#include <vector>

namespace chip8
{
    constexpr int32_t REGISTER_COUNT = 16;

    class cRam;

    class cProcessor
    {
      public:
        cProcessor(int32_t program_start_location, int32_t register_count);

        void execute_next_instruction(chip8::cRam* ram);

      private:
        void execute_opcode_0XXX(int16_t opcode);
        void execute_opcode_1NNN(int16_t opcode);
        void execute_opcode_2NNN(int16_t opcode);
        void execute_opcode_3XNN(int16_t opcode);
        void execute_opcode_4XNN(int16_t opcode);
        void execute_opcode_5XY0(int16_t opcode);
        void execute_opcode_6XNN(int16_t opcode);
        void execute_opcode_7XNN(int16_t opcode);
        void execute_opcode_8XXX(int16_t opcode);
        void execute_opcode_9XY0(int16_t opcode);
        void execute_opcode_ANNN(int16_t opcode);
        void execute_opcode_BNNN(int16_t opcode);
        void execute_opcode_CXNN(int16_t opcode);
        void execute_opcode_DXYN(int16_t opcode);
        void execute_opcode_EXXX(int16_t opcode);
        void execute_opcode_FXXX(int16_t opcode);

        int16_t              _program_counter;
        int16_t              _register_i;
        std::vector<uint8_t> _registers;
    };
}

#endif // CHIP8_SRC_PROCESSORHPP
