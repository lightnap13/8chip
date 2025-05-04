#include "processor.hpp"

#include "display.hpp"
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

    void cProcessor::execute_next_instruction(cRam* ram, cDisplay* display)
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
                execute_opcode_0XXX(opcode, display);
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
                execute_opcode_DXYN(opcode, ram, display);
            }
            break;
            case 0x0E:
            {
                // Needs keyboard.
                execute_opcode_EXXX(opcode);
            }
            break;
            case 0x0F:
            {
                // Needs timers
                // Needs keyboard.
                execute_opcode_FXXX(opcode, ram);
            }
            break;
            default:
            {
                thoth::error("Invalid operation %x04", opcode);
                std::abort();
            }
            break;
        }
    }

    void cProcessor::execute_opcode_0XXX(int16_t opcode, cDisplay* display)
    {
        // Ignore 0NNNN for now

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
            execute_opcode_0E00(opcode, display);
        }
        else
        {
            thoth::error("Invalid operation %x04", opcode);
            std::abort();
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
                std::abort();
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
            std::abort();
        }
    }
    void cProcessor::execute_opcode_FXXX(int16_t opcode, cRam* ram)
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
            execute_opcode_FX55(opcode, ram);
        }
        else if (nibble3 == 0x6 && nibble4 == 0x5)
        {
            execute_opcode_FX65(opcode, ram);
        }
        else
        {
            thoth::error("Invalid operation %x04", opcode);
            std::abort();
        }
    }

    void cProcessor::execute_opcode_0E00(int16_t opcode, cDisplay* display)
    {
        // Clears screen.
    }
    void cProcessor::execute_opcode_00EE(int16_t opcode)
    {
        // Return from function
    }
    void cProcessor::execute_opcode_1NNN(int16_t opcode)
    {
        // Jumo to adress NNN,
    }
    void cProcessor::execute_opcode_2NNN(int16_t opcode)
    {
        // Cals subroutine at NNN
    }
    void cProcessor::execute_opcode_3XNN(int16_t opcode)
    {
        // If vx == NN, skip next instruction
    }
    void cProcessor::execute_opcode_4XNN(int16_t opcode)
    {
        // If vx != NN, skip the next instruction
    }
    void cProcessor::execute_opcode_5XY0(int16_t opcode)
    {
        // Skip next instruction if Vx == Vy
    }
    void cProcessor::execute_opcode_6XNN(int16_t opcode)
    {
        // Set VX to NN
    }
    void cProcessor::execute_opcode_7XNN(int16_t opcode)
    {
        // Adds NN to Vx (carry flag is not changed)
    }
    void cProcessor::execute_opcode_8XY0(int16_t opcode)
    {
        // Vx = Vy
    }
    void cProcessor::execute_opcode_8XY1(int16_t opcode)
    {
        // Vx |= Vy
    }
    void cProcessor::execute_opcode_8XY2(int16_t opcode)
    {
        // Vx &= Vy
    }
    void cProcessor::execute_opcode_8XY3(int16_t opcode)
    {
        // Vx ^= Vy
    }
    void cProcessor::execute_opcode_8XY4(int16_t opcode)
    {
        // Vx += Vy. Set VF to 1 if there is overflow, to 0 if not.
    }
    void cProcessor::execute_opcode_8XY5(int16_t opcode)
    {
        // Vx -= Vy. Vf set to 0 if there is underflow, to 0 if not (VF set to 1 if Vx >= Vy)
    }
    void cProcessor::execute_opcode_8XY6(int16_t opcode)
    {
        // Vx >>= 1. Store least significand bit of Vx prior to shift to VF.
    }
    void cProcessor::execute_opcode_8XY7(int16_t opcode)
    {
        // Vx = Vy - Vx. Vf set to 0 if Vy >= Vx, to 1 otherwise.
    }
    void cProcessor::execute_opcode_8XYE(int16_t opcode)
    {
        // Vx <<= 1. Set Vf to 1 if most significanf bit of Vx prior to shift was set, to 0 otherwise.
    }

    void cProcessor::execute_opcode_9XY0(int16_t opcode)
    {
        // Skip next instruction if Vx != Vy
    }
    void cProcessor::execute_opcode_ANNN(int16_t opcode)
    {
        // I = NNN
    }
    void cProcessor::execute_opcode_BNNN(int16_t opcode)
    {
        // PC = V0 + NNN
    }
    void cProcessor::execute_opcode_CXNN(int16_t opcode)
    {
        // Vx = rand(0,255) & NN
    }
    void cProcessor::execute_opcode_DXYN(int16_t opcode, cRam* ram, cDisplay* display)
    {
        // draw(vx, vy, N). Draw a sprite at coordinate Vx, Vy
        // that has a width of 8 picels and a height of N pixels.
        // Each row of 8 pixels is read as bit coded starting from memory location I.
        // I does no change.
        // Set VF to 1 if any bits are flipped from set to unset when drawing, 0 if that does not happen.
    }
    void cProcessor::execute_opcode_EX9E(int16_t opcode)
    {
        // Skip next instruction if key stored in Vx (consider only lowest nibble (half-bit)) is pressed.
    }
    void cProcessor::execute_opcode_EXA1(int16_t opcode)
    {
        // Skip next instruction if key stored in Vx (consider only lowest nibble (half-bit)) is NOT pressed.
    }
    void cProcessor::execute_opcode_FX07(int16_t opcode)
    {
        // Sets vx to the current value of the delay timer
    }
    void cProcessor::execute_opcode_FX0A(int16_t opcode)
    {
        // A key press is awaited, then stored in Vx
        // Blocking operation, all instructions halted until next event
        // But sound and delay timers keep processing.
    }
    void cProcessor::execute_opcode_FX15(int16_t opcode)
    {
        // Sets delay timer to Vx
    }
    void cProcessor::execute_opcode_FX18(int16_t opcode)
    {
        // Sets sound timer to Vx
    }
    void cProcessor::execute_opcode_FX1E(int16_t opcode)
    {
        // I += Vx. Vf is not affected.
    }
    void cProcessor::execute_opcode_FX29(int16_t opcode)
    {
        // Okay so somewhere in memory are sprites for the characters 0-9 and A-F, in 4x5 font.
        // This instruction read the lowest nibble (half bit) of Vx and then sets I to the location
        // In memory where you can find the sprite for said character.
        // Should I provide this or....
        //
        // Does not really require memory, just a table that says where these sprites are located.
    }
    void cProcessor::execute_opcode_FX33(int16_t opcode)
    {
        // Need memory.
        // Takes the number in Vx. Reads it as in decimal. Like if Vx is OxE7 it considers it 231.
        // Then stores 231 in I, like it stores 2 in I, 3 in I+1 and 1 in I+2.
        // This is called binary coded decimal.
    }
    void cProcessor::execute_opcode_FX55(int16_t opcode, cRam* ram)
    {
        // Stores from V0 to Vx (including Vx) in memory, starting at adress I.
        // I is not modified.
    }
    void cProcessor::execute_opcode_FX65(int16_t opcode, cRam* ram)
    {
        // Fills V0 to Vx (including Vx) with values from memory, starting at adress I.
        // I is not modified.
    }
}
