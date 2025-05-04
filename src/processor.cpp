#include "processor.hpp"

#include "log.hpp"
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

        // Opcode = Nibble 1234
        uint8_t nibble1 = (opcode >> 12) & 0x000F;

        thoth::info("Executing instruction %04x at program counter=%d\n", opcode, _program_counter - 2);

        switch (nibble1)
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
                thoth::error("Invalid operation %x04", opcode);
                // TODO: Thow or something.
            }
            break;
        }
    }

    void cProcessor::execute_opcode_0XXX(int16_t opcode)
    {
        // Ignore 0NNNN for now?
        // Opcode = Nibble 1234
        uint8_t nibble2 = (opcode >> 8) & 0x000F;
        uint8_t nibble3 = (opcode >> 4) & 0x000F;
        uint8_t nibble4 = opcode & 0x000F;

        if (nibble2 == 0x0 && nibble3 == 0xE && nibble4 == 0xE)
        {
            execute_opcode_00EE(opcode);
        }
        else if (nibble2 == 0xE && nibble3 == 0x0 && nibble4 == 0x0)
        {
            execute_opcode_0E00(opcode);
        }
        else
        {
            thoth::error("Invalid operation %x04", opcode);
            // TODO: Throw or something.
        }
    }

    void cProcessor::execute_opcode_8XXX(int16_t opcode)
    {
        // Opcode = Nibble 1234
        uint8_t nibble4 = opcode & 0x000F;

        switch (nibble4)
        {
            case 0x00:
            {
                execute_opcode_8XY0(opcode);
            }
            break;
            case 0x01:
            {
                execute_opcode_8XY1(opcode);
            }
            break;
            case 0x02:
            {
                execute_opcode_8XY2(opcode);
            }
            break;
            case 0x03:
            {
                execute_opcode_8XY3(opcode);
            }
            break;
            case 0x04:
            {
                execute_opcode_8XY4(opcode);
            }
            break;
            case 0x05:
            {
                execute_opcode_8XY5(opcode);
            }
            break;
            case 0x06:
            {
                execute_opcode_8XY6(opcode);
            }
            break;
            case 0x07:
            {
                execute_opcode_8XY7(opcode);
            }
            break;
            case 0x0E:
            {
                execute_opcode_8XYE(opcode);
            }
            break;
            default:
            {
                thoth::error("Invalid operation %x04", opcode);
                // TODO: Thow or something.
            }
            break;
        }
    }

    void cProcessor::execute_opcode_EXXX(int16_t opcode)
    {
        // Opcode = Nibble 1234
        uint8_t nibble3 = (opcode >> 4) & 0x000F;
        uint8_t nibble4 = opcode & 0x000F;

        if (nibble3 == 0x9 && nibble4 == 0xE)
        {
            execute_opcode_EX9E(opcode);
        }
        else if (nibble3 == 0xA && nibble4 == 0x1)
        {
            execute_opcode_EXA1(opcode);
        }
        else
        {
            thoth::error("Invalid operation %x04", opcode);
            // TODO: Throw or something.
        }
    }
    void cProcessor::execute_opcode_FXXX(int16_t opcode)
    {
        // Opcode = Nibble 1234
        uint8_t nibble3 = (opcode >> 4) & 0x000F;
        uint8_t nibble4 = opcode & 0x000F;

        if (nibble3 == 0x0 && nibble4 == 0x7)
        {
            execute_opcode_FX07(opcode);
        }
        else if (nibble3 == 0x0 && nibble4 == 0xA)
        {
            execute_opcode_FX0A(opcode);
        }
        else if (nibble3 == 0x1 && nibble4 == 0x5)
        {
            execute_opcode_FX15(opcode);
        }
        else if (nibble3 == 0x1 && nibble4 == 0x8)
        {
            execute_opcode_FX18(opcode);
        }
        else if (nibble3 == 0x1 && nibble4 == 0xE)
        {
            execute_opcode_FX1E(opcode);
        }
        else if (nibble3 == 0x2 && nibble4 == 0x9)
        {
            execute_opcode_FX29(opcode);
        }
        else if (nibble3 == 0x3 && nibble4 == 0x3)
        {
            execute_opcode_FX33(opcode);
        }
        else if (nibble3 == 0x5 && nibble4 == 0x5)
        {
            execute_opcode_FX55(opcode);
        }
        else if (nibble3 == 0x6 && nibble4 == 0x5)
        {
            execute_opcode_FX65(opcode);
        }
        else
        {
            thoth::error("Invalid operation %x04", opcode);
            // TODO: Throw or something,
        }
    }

    void cProcessor::execute_opcode_0E00(int16_t opcode) {}
    void cProcessor::execute_opcode_00EE(int16_t opcode) {}
    void cProcessor::execute_opcode_1NNN(int16_t opcode) {}
    void cProcessor::execute_opcode_2NNN(int16_t opcode) {}
    void cProcessor::execute_opcode_3XNN(int16_t opcode) {}
    void cProcessor::execute_opcode_4XNN(int16_t opcode) {}
    void cProcessor::execute_opcode_5XY0(int16_t opcode) {}
    void cProcessor::execute_opcode_6XNN(int16_t opcode) {}
    void cProcessor::execute_opcode_7XNN(int16_t opcode) {}
    void cProcessor::execute_opcode_8XY0(int16_t opcode) {}
    void cProcessor::execute_opcode_8XY1(int16_t opcode) {}
    void cProcessor::execute_opcode_8XY2(int16_t opcode) {}
    void cProcessor::execute_opcode_8XY3(int16_t opcode) {}
    void cProcessor::execute_opcode_8XY4(int16_t opcode) {}
    void cProcessor::execute_opcode_8XY5(int16_t opcode) {}
    void cProcessor::execute_opcode_8XY6(int16_t opcode) {}
    void cProcessor::execute_opcode_8XY7(int16_t opcode) {}
    void cProcessor::execute_opcode_8XYE(int16_t opcode) {}
    void cProcessor::execute_opcode_9XY0(int16_t opcode) {}
    void cProcessor::execute_opcode_ANNN(int16_t opcode) {}
    void cProcessor::execute_opcode_BNNN(int16_t opcode) {}
    void cProcessor::execute_opcode_CXNN(int16_t opcode) {}
    void cProcessor::execute_opcode_DXYN(int16_t opcode) {}
    void cProcessor::execute_opcode_EX9E(int16_t opcode) {}
    void cProcessor::execute_opcode_EXA1(int16_t opcode) {}
    void cProcessor::execute_opcode_FX07(int16_t opcode) {}
    void cProcessor::execute_opcode_FX0A(int16_t opcode) {}
    void cProcessor::execute_opcode_FX15(int16_t opcode) {}
    void cProcessor::execute_opcode_FX18(int16_t opcode) {}
    void cProcessor::execute_opcode_FX1E(int16_t opcode) {}
    void cProcessor::execute_opcode_FX29(int16_t opcode) {}
    void cProcessor::execute_opcode_FX33(int16_t opcode) {}
    void cProcessor::execute_opcode_FX55(int16_t opcode) {}
    void cProcessor::execute_opcode_FX65(int16_t opcode) {}
}
