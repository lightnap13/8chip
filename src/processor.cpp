#include "processor.hpp"

#include "display.hpp"
#include "keyboard.hpp"
#include "log.hpp"
#include "ram.hpp"
#include "timer.hpp"

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

    void cProcessor::execute_next_instruction(cRam* ram, cDisplay* display, cKeyboard* keyboard, cTimer* delay_timer, cTimer* sound_timer)
    {
        assert(_program_counter < ram->size() - 1);
        uint16_t instr_first_half = static_cast<uint16_t>(ram->read(_program_counter));
        uint16_t instr_second_half = static_cast<uint16_t>(ram->read(_program_counter + 1));

        uint16_t opcode = (instr_first_half << 8) | instr_second_half;

        // Opcode = Nibble 1234
        uint8_t nibble1 = (opcode >> 12) & 0x000F;

        thoth::info("Executing instruction %04x at program counter=%d\n", opcode, _program_counter);

        _program_counter += 2;

        switch (nibble1)
        {
            case 0x00:
            {
                execute_opcode_0XXX(opcode, ram, display);
            }
            break;
            case 0x01:
            {
                execute_opcode_1NNN(opcode);
            }
            break;
            case 0x02:
            {
                execute_opcode_2NNN(opcode, ram);
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
                execute_opcode_EXXX(opcode, keyboard);
            }
            break;
            case 0x0F:
            {
                // Needs timers
                // Needs keyboard.
                execute_opcode_FXXX(opcode, ram, keyboard, delay_timer, sound_timer);
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

    void cProcessor::execute_opcode_0XXX(int16_t opcode, cRam* ram, cDisplay* display)
    {
        // Ignore 0NNNN for now

        // Opcode = Nibble 1234
        uint8_t nibble2 = (opcode >> 8) & 0x000F;
        uint8_t nibble3 = (opcode >> 4) & 0x000F;
        uint8_t nibble4 = opcode & 0x000F;

        if (nibble2 == 0x0 && nibble3 == 0xE && nibble4 == 0xE)
        {
            execute_opcode_00EE(opcode, ram);
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

    void cProcessor::execute_opcode_EXXX(int16_t opcode, cKeyboard* keyboard)
    {
        // Opcode = Nibble 1234
        uint8_t nibble3 = (opcode >> 4) & 0x000F;
        uint8_t nibble4 = opcode & 0x000F;

        if (nibble3 == 0x9 && nibble4 == 0xE)
        {
            execute_opcode_EX9E(opcode, keyboard);
        }
        else if (nibble3 == 0xA && nibble4 == 0x1)
        {
            execute_opcode_EXA1(opcode, keyboard);
        }
        else
        {
            thoth::error("Invalid operation %x04", opcode);
            std::abort();
        }
    }
    void cProcessor::execute_opcode_FXXX(int16_t opcode, cRam* ram, cKeyboard* keyboard, cTimer* delay_timer, cTimer* sound_timer)
    {
        // Opcode = Nibble 1234
        uint8_t nibble3 = (opcode >> 4) & 0x000F;
        uint8_t nibble4 = opcode & 0x000F;

        if (nibble3 == 0x0 && nibble4 == 0x7)
        {
            execute_opcode_FX07(opcode, delay_timer);
        }
        else if (nibble3 == 0x0 && nibble4 == 0xA)
        {
            execute_opcode_FX0A(opcode, keyboard);
        }
        else if (nibble3 == 0x1 && nibble4 == 0x5)
        {
            execute_opcode_FX15(opcode, delay_timer);
        }
        else if (nibble3 == 0x1 && nibble4 == 0x8)
        {
            execute_opcode_FX18(opcode, sound_timer);
        }
        else if (nibble3 == 0x1 && nibble4 == 0xE)
        {
            execute_opcode_FX1E(opcode);
        }
        else if (nibble3 == 0x2 && nibble4 == 0x9)
        {
            execute_opcode_FX29(opcode, ram);
        }
        else if (nibble3 == 0x3 && nibble4 == 0x3)
        {
            execute_opcode_FX33(opcode, ram);
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
        display->clear_pixels();
    }

    void cProcessor::execute_opcode_00EE(int16_t opcode, cRam* ram)
    {
        // Return from function
        _program_counter = ram->pop_from_stack();
    }

    void cProcessor::execute_opcode_1NNN(int16_t opcode)
    {
        // Jumps to NNN.
        uint16_t jump_position = opcode & 0x0FFF;
        _program_counter = jump_position;
    }

    void cProcessor::execute_opcode_2NNN(int16_t opcode, cRam* ram)
    {
        // Calls subroutine at NNN
        uint16_t jump_position = opcode & 0x0FFF;
        ram->push_to_stack(_program_counter);
        _program_counter = jump_position;
    }

    void cProcessor::execute_opcode_3XNN(int16_t opcode)
    {
        // If vx == NN, skip next instruction
        size_t  register_index = (opcode >> 8) & 0x0F;
        uint8_t register_content = _registers[register_index];
        uint8_t constant = opcode & 0xFF;
        if (register_content == constant)
        {
            _program_counter += 2U;
        }
    }

    void cProcessor::execute_opcode_4XNN(int16_t opcode)
    {
        // If vx != NN, skip the next instruction
        size_t  register_index = (opcode >> 8) & 0x0F;
        uint8_t register_content = _registers[register_index];
        uint8_t constant = opcode & 0xFF;
        if (register_content != constant)
        {
            _program_counter += 2U;
        }
    }

    void cProcessor::execute_opcode_5XY0(int16_t opcode)
    {
        // Skip next instruction if Vx == Vy
        size_t  register_index_x = (opcode >> 8) & 0x0F;
        uint8_t register_content_x = _registers[register_index_x];
        size_t  register_index_y = (opcode >> 4) & 0x0F;
        uint8_t register_content_y = _registers[register_index_y];
        if (register_content_x == register_content_y)
        {
            _program_counter += 2U;
        }
    }

    void cProcessor::execute_opcode_6XNN(int16_t opcode)
    {
        // Set VX to NN
        size_t  register_index = (opcode >> 8) & 0x0F;
        uint8_t constant = opcode & 0xFF;
        _registers[register_index] = constant;
    }

    void cProcessor::execute_opcode_7XNN(int16_t opcode)
    {
        // Adds NN to Vx (carry flag is not changed)
        size_t  register_index = (opcode >> 8) & 0x0F;
        uint8_t constant = opcode & 0xFF;
        _registers[register_index] += constant;
    }

    void cProcessor::execute_opcode_8XY0(int16_t opcode)
    {
        // Vx = Vy
        size_t  register_index_x = (opcode >> 8) & 0x0F;
        size_t  register_index_y = (opcode >> 4) & 0x0F;
        uint8_t register_content_y = _registers[register_index_y];
        _registers[register_index_x] = register_content_y;
    }

    void cProcessor::execute_opcode_8XY1(int16_t opcode)
    {
        // Vx |= Vy
        size_t  register_index_x = (opcode >> 8) & 0x0F;
        size_t  register_index_y = (opcode >> 4) & 0x0F;
        uint8_t register_content_y = _registers[register_index_y];
        _registers[register_index_x] |= register_content_y;
    }

    void cProcessor::execute_opcode_8XY2(int16_t opcode)
    {
        // Vx &= Vy
        size_t  register_index_x = (opcode >> 8) & 0x0F;
        size_t  register_index_y = (opcode >> 4) & 0x0F;
        uint8_t register_content_y = _registers[register_index_y];
        _registers[register_index_x] &= register_content_y;
    }

    void cProcessor::execute_opcode_8XY3(int16_t opcode)
    {
        // Vx ^= Vy
        size_t  register_index_x = (opcode >> 8) & 0x0F;
        size_t  register_index_y = (opcode >> 4) & 0x0F;
        uint8_t register_content_y = _registers[register_index_y];
        _registers[register_index_x] ^= register_content_y;
    }

    void cProcessor::execute_opcode_8XY4(int16_t opcode)
    {
        // Vx += Vy. Set VF to 1 if there is overflow, to 0 if not.
        size_t  register_index_x = (opcode >> 8) & 0x0F;
        uint8_t register_content_x = _registers[register_index_x];
        size_t  register_index_y = (opcode >> 4) & 0x0F;
        uint8_t register_content_y = _registers[register_index_y];
        uint8_t sum = register_content_x + register_content_y;
        _registers[register_index_x] = sum;
        if (sum < register_content_x)
        {
            _registers[15] = 1;
        }
        else
        {
            _registers[15] = 0;
        }
    }

    void cProcessor::execute_opcode_8XY5(int16_t opcode)
    {
        // Vx -= Vy. Vf set to 0 if there is underflow, to 0 if not (VF set to 1 if Vx >= Vy)
        size_t  register_index_x = (opcode >> 8) & 0x0F;
        uint8_t register_content_x = _registers[register_index_x];
        size_t  register_index_y = (opcode >> 4) & 0x0F;
        uint8_t register_content_y = _registers[register_index_y];
        uint8_t diff = register_content_x - register_content_y;
        _registers[register_index_x] = diff;

        if (register_content_x >= register_content_y)
        {
            _registers[15] = 1;
        }
        else
        {
            _registers[15] = 0;
        }
    }

    void cProcessor::execute_opcode_8XY6(int16_t opcode)
    {
        // Vx >>= 1. Store least significand bit of Vx prior to shift to VF.
        size_t  register_index = (opcode >> 8) & 0x0F;
        uint8_t register_content = _registers[register_index];
        _registers[15] = register_content & 0x01;
        _registers[register_index] = register_content >> 1;
    }

    void cProcessor::execute_opcode_8XY7(int16_t opcode)
    {
        // Vx = Vy - Vx. Vf set to 0 if Vy >= Vx, to 1 otherwise.
        size_t  register_index_x = (opcode >> 8) & 0x0F;
        uint8_t register_content_x = _registers[register_index_x];
        size_t  register_index_y = (opcode >> 4) & 0x0F;
        uint8_t register_content_y = _registers[register_index_y];
        uint8_t diff = register_content_y - register_content_x;
        _registers[register_index_x] = diff;

        if (register_content_y >= register_content_x)
        {
            _registers[15] = 1;
        }
        else
        {
            _registers[15] = 0;
        }
    }

    void cProcessor::execute_opcode_8XYE(int16_t opcode)
    {
        // Vx <<= 1. Set Vf to 1 if most significanf bit of Vx prior to shift was set, to 0 otherwise.
        size_t  register_index = (opcode >> 8) & 0x0F;
        uint8_t register_content = _registers[register_index];
        _registers[15] = (register_content & 0x80) >> 7;
        _registers[register_index] = register_content << 1;
    }

    void cProcessor::execute_opcode_9XY0(int16_t opcode)
    {
        // Skip next instruction if Vx != Vy
        size_t  register_index_x = (opcode >> 8) & 0x0F;
        uint8_t register_content_x = _registers[register_index_x];
        size_t  register_index_y = (opcode >> 4) & 0x0F;
        uint8_t register_content_y = _registers[register_index_y];
        if (register_content_x != register_content_y)
        {
            _program_counter += 2U;
        }
    }

    void cProcessor::execute_opcode_ANNN(int16_t opcode)
    {
        // I = NNN
        uint16_t constant = opcode & 0x0FFF;
        _register_i = constant;
    }

    void cProcessor::execute_opcode_BNNN(int16_t opcode)
    {
        // PC = V0 + NNN
        uint16_t constant = opcode & 0x0FFF;
        _program_counter = static_cast<uint16_t>(_registers[0]) + constant;
    }

    void cProcessor::execute_opcode_CXNN(int16_t opcode)
    {
        // Vx = rand(0,255) & NN
        uint8_t constant = opcode & 0x00FF;
        size_t  register_index = (opcode >> 8) & 0x0F;
        _registers[register_index] = static_cast<uint8_t>((rand() % 255)) & constant;
    }

    void cProcessor::execute_opcode_DXYN(int16_t opcode, cRam* ram, cDisplay* display)
    {
        // TODO: This operation.
        // draw(vx, vy, N). Draw a sprite at coordinate Vx, Vy
        // that has a width of 8 pixels and a height of N pixels.
        // Each row of 8 pixels is read as bit coded starting from memory location I.
        // I does no change.
        // Set VF to 1 if any bits are flipped from set to unset when drawing, 0 if that does not happen.
    }

    void cProcessor::execute_opcode_EX9E(int16_t opcode, cKeyboard* keyboard)
    {
        // Skip next instruction if key stored in Vx (consider only lowest nibble (half-bit)) is pressed.
        size_t register_index = (opcode >> 8) & 0x0F;
        bool   key_pressed = keyboard->is_key_pressed(_registers[register_index]);
        if (key_pressed)
        {
            _program_counter += 2;
        }
    }

    void cProcessor::execute_opcode_EXA1(int16_t opcode, cKeyboard* keyboard)
    {
        // Skip next instruction if key stored in Vx (consider only lowest nibble (half-bit)) is NOT pressed.
        size_t register_index = (opcode >> 8) & 0x0F;
        bool   key_pressed = keyboard->is_key_pressed(_registers[register_index]);
        if (!key_pressed)
        {
            _program_counter += 2;
        }
    }

    void cProcessor::execute_opcode_FX07(int16_t opcode, cTimer* delay_timer)
    {
        // Sets vx to the current value of the delay timer
        size_t register_index = (opcode >> 8) & 0x0F;
        _registers[register_index] = delay_timer->get_time();
    }

    void cProcessor::execute_opcode_FX0A(int16_t opcode, cKeyboard* keyboard)
    {
        // A key press is awaited, then stored in Vx
        // If no key is pressed we decrement program counter as to execute this instruction again.
        // But sound and delay timers keep processing.
        int8_t pressed_key = keyboard->await_key_press();

        if (pressed_key == -1)
        {
            _program_counter -= 2;
            return;
        }
        else
        {
            size_t register_index = (opcode >> 8) & 0x0F;
            _registers[register_index] = static_cast<uint8_t>(pressed_key);
            return;
        }
    }

    void cProcessor::execute_opcode_FX15(int16_t opcode, cTimer* delay_timer)
    {
        // Sets delay timer to Vx
        size_t register_index = (opcode >> 8) & 0x0F;
        delay_timer->set_time(_registers[register_index]);
    }

    void cProcessor::execute_opcode_FX18(int16_t opcode, cTimer* sound_timer)
    {
        // Sets sound timer to Vx
        size_t register_index = (opcode >> 8) & 0x0F;
        sound_timer->set_time(_registers[register_index]);
    }

    void cProcessor::execute_opcode_FX1E(int16_t opcode)
    {
        // I += Vx. Vf is not affected.
        size_t register_index = (opcode >> 8) & 0x0F;
        _register_i += _registers[register_index];

        if (_register_i >= 4096)
        {
            _register_i -= 4096;
        }

        if (_register_i < 0)
        {
            _register_i += 4096;
        }
    }

    void cProcessor::execute_opcode_FX29(int16_t opcode, cRam* ram)
    {
        // Okay so somewhere in memory are sprites for the characters 0-9 and A-F, in 4x5 font.
        // This instruction read the lowest nibble (half bit) of Vx and then sets I to the location
        // In memory where you can find the sprite for said character.
        uint8_t  character = opcode & 0xF;
        uint16_t position = ram->get_font_char_position(character);
        _register_i = position;
    }

    void cProcessor::execute_opcode_FX33(int16_t opcode, cRam* ram)
    {
        // Takes the number in Vx. Reads it as in decimal. Like if Vx is OxE7 it considers it 231.
        // Then stores 231 in I, like it stores 2 in I, 3 in I+1 and 1 in I+2.
        // This is called binary coded decimal.
        size_t  register_index = (opcode >> 8) & 0x0F;
        uint8_t register_content = _registers[register_index];
        uint8_t units = register_content % 10;
        uint8_t tens = (register_content % 100) / 10;
        uint8_t hundreds = register_content / 100;
        ram->write(_register_i, hundreds);
        ram->write(_register_i + 1, tens);
        ram->write(_register_i + 2, units);
    }

    void cProcessor::execute_opcode_FX55(int16_t opcode, cRam* ram)
    {
        // Stores from V0 to Vx (including Vx) in memory, starting at adress I.
        // I is not modified.
        size_t register_index = (opcode >> 8) & 0x0F;
        for (size_t i {0U}; i <= register_index; i++)
        {
            ram->write(_register_i + i, _registers[i]);
        }
    }

    void cProcessor::execute_opcode_FX65(int16_t opcode, cRam* ram)
    {
        // Fills V0 to Vx (including Vx) with values from memory, starting at adress I.
        // I is not modified.
        size_t register_index = (opcode >> 8) & 0x0F;
        for (size_t i {0U}; i <= register_index; i++)
        {
            _registers[i] = ram->read(_register_i + i);
        }
    }
}
