#include "processor.hpp"

#include "ram.hpp"

#include <assert.h>
#include <cstdio>

namespace chip8
{
    cProcessor::cProcessor(int32_t program_start_location, int32_t register_count)
    {
        _registers = std::vector<uint8_t>(register_count, 0U);
        _register_i = 0U;
        _program_counter = static_cast<int16_t>(program_start_location);
    }

    void cProcessor::execute_next_instruction(chip8::cRam* ram)
    {
        assert(_program_counter < ram->size() - 1);
        uint16_t instr_first_half = static_cast<uint16_t>(ram->read(_program_counter));
        uint16_t instr_second_half = static_cast<uint16_t>(ram->read(_program_counter + 1));

        _program_counter += 2;
        uint16_t opcode = (instr_first_half << 8) | instr_second_half;

        uint8_t first_4_bits = (opcode >> 12) & 0x000F;

        std::printf("[ INFO] Executing instruction %04x at program counter=%d\n", opcode, _program_counter - 2);

        switch (first_4_bits)
        {
            case 0x00:
            {
                execute_opcode_0XXX(opcode);
            }
            break;
            case 0x01:
            {
                execute_opcode_1NNN(opcode);
            }
            break;
            case 0x02:
            {
                execute_opcode_2NNN(opcode);
            }
            break;
            case 0x03:
            {
                execute_opcode_3XNN(opcode);
            }
            break;
            case 0x04:
            {
                execute_opcode_4XNN(opcode);
            }
            break;
            case 0x05:
            {
                execute_opcode_5XY0(opcode);
            }
            break;
            case 0x06:
            {
                execute_opcode_6XNN(opcode);
            }
            break;
            case 0x07:
            {
                execute_opcode_7XNN(opcode);
            }
            break;
            case 0x08:
            {
                execute_opcode_8XXX(opcode);
            }
            break;
            case 0x09:
            {
                execute_opcode_9XY0(opcode);
            }
            break;
            case 0x0A:
            {
                execute_opcode_ANNN(opcode);
            }
            break;
            case 0x0B:
            {
                execute_opcode_BNNN(opcode);
            }
            break;
            case 0x0C:
            {
                execute_opcode_CXNN(opcode);
            }
            break;
            case 0x0D:
            {
                execute_opcode_DXYN(opcode);
            }
            break;
            case 0x0E:
            {
                execute_opcode_EXXX(opcode);
            }
            break;
            case 0x0F:
            {
                execute_opcode_FXXX(opcode);
            }
            break;
            default:
            {
                // TODO: Thow or something.
            }
            break;
        }
    }

    void cProcessor::execute_opcode_0XXX(int16_t opcode) {}
    void cProcessor::execute_opcode_1NNN(int16_t opcode) {}
    void cProcessor::execute_opcode_2NNN(int16_t opcode) {}
    void cProcessor::execute_opcode_3XNN(int16_t opcode) {}
    void cProcessor::execute_opcode_4XNN(int16_t opcode) {}
    void cProcessor::execute_opcode_5XY0(int16_t opcode) {}
    void cProcessor::execute_opcode_6XNN(int16_t opcode) {}
    void cProcessor::execute_opcode_7XNN(int16_t opcode) {}
    void cProcessor::execute_opcode_8XXX(int16_t opcode) {}
    void cProcessor::execute_opcode_9XY0(int16_t opcode) {}
    void cProcessor::execute_opcode_ANNN(int16_t opcode) {}
    void cProcessor::execute_opcode_BNNN(int16_t opcode) {}
    void cProcessor::execute_opcode_CXNN(int16_t opcode) {}
    void cProcessor::execute_opcode_DXYN(int16_t opcode) {}
    void cProcessor::execute_opcode_EXXX(int16_t opcode) {}
    void cProcessor::execute_opcode_FXXX(int16_t opcode) {}
}
