#include "processor.hpp"

#include "display.hpp"
#include "keyboard.hpp"
#include "log.hpp"
#include "ram.hpp"
#include "timer.hpp"

#include <assert.h>
#include <cstdio>

namespace
{
    uint8_t inline get_nibble(uint16_t opcode, uint8_t nibble_index) noexcept
    {
        assert(0 <= nibble_index && nibble_index <= 3);
        // Opcode = (Nibble3, Nibble2, Nibble1, Nibble0)
        return (opcode >> 4 * nibble_index) & 0x000F;
    }

}

namespace chip8
{
    cProcessor::cProcessor(int32_t program_start_location, int32_t register_count) noexcept
      : _program_counter(static_cast<int16_t>(program_start_location))
      , _register_i(0U)
      , _registers(register_count, 0U)
    {
    }

    void cProcessor::execute_next_instruction(cRam* ram, cDisplay* display, cKeyboard* keyboard, cTimer* delay_timer, cTimer* sound_timer) & noexcept
    {
        assert(_program_counter < ram->size() - 1);
        uint16_t const instr_first_half {static_cast<uint16_t>(ram->read(_program_counter))};
        uint16_t const instr_second_half {static_cast<uint16_t>(ram->read(_program_counter + 1))};
        uint16_t const opcode {static_cast<uint16_t>((instr_first_half << 8) | instr_second_half)};

        thoth::info("Executing instruction %04x at program counter=%d\n", opcode, _program_counter);

        _program_counter += 2;

        switch (get_nibble(opcode, 3))
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
                thoth::fatal("Invalid operation %04x", opcode);
                std::abort();
            }
            break;
        }
    }

    void cProcessor::execute_opcode_0XXX(uint16_t opcode, cRam* ram, cDisplay* display) & noexcept
    {

        uint8_t const nibble2 {get_nibble(opcode, 2)};
        uint8_t const nibble1 {get_nibble(opcode, 1)};
        uint8_t const nibble0 {get_nibble(opcode, 0)};

        // Ignore 0NNN for now
        if (nibble2 == 0x0 && nibble1 == 0xE && nibble0 == 0xE)
        {
            execute_opcode_00EE(opcode, ram);
        }
        else if (nibble2 == 0x0 && nibble1 == 0xE && nibble0 == 0x0)
        {
            execute_opcode_00E0(opcode, display);
        }
        else
        {
            thoth::fatal("Invalid operation of type 0XXX: %04x", opcode);
            std::abort();
        }
    }

    void cProcessor::execute_opcode_8XXX(uint16_t opcode) & noexcept
    {
        switch (get_nibble(opcode, 0))
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
                thoth::fatal("Invalid operation of type 8XXX: %04x", opcode);
                std::abort();
            }

            break;
        }
    }

    void cProcessor::execute_opcode_EXXX(uint16_t opcode, cKeyboard* keyboard) & noexcept
    {
        uint8_t const nibble1 {get_nibble(opcode, 1)};
        uint8_t const nibble0 {get_nibble(opcode, 0)};

        if (nibble1 == 0x9 && nibble0 == 0xE)
        {
            execute_opcode_EX9E(opcode, keyboard);
        }
        else if (nibble1 == 0xA && nibble0 == 0x1)
        {
            execute_opcode_EXA1(opcode, keyboard);
        }
        else
        {
            thoth::fatal("Invalid operation of type EXXX: %04x", opcode);
            std::abort();
        }
    }

    void cProcessor::execute_opcode_FXXX(uint16_t opcode, cRam* ram, cKeyboard* keyboard, cTimer* delay_timer, cTimer* sound_timer) & noexcept
    {
        uint8_t const nibble1 {get_nibble(opcode, 1)};
        uint8_t const nibble0 {get_nibble(opcode, 0)};

        if (nibble1 == 0x0 && nibble0 == 0x7)
        {
            execute_opcode_FX07(opcode, delay_timer);
        }
        else if (nibble1 == 0x0 && nibble0 == 0xA)
        {
            execute_opcode_FX0A(opcode, keyboard);
        }
        else if (nibble1 == 0x1 && nibble0 == 0x5)
        {
            execute_opcode_FX15(opcode, delay_timer);
        }
        else if (nibble1 == 0x1 && nibble0 == 0x8)
        {
            execute_opcode_FX18(opcode, sound_timer);
        }
        else if (nibble1 == 0x1 && nibble0 == 0xE)
        {
            execute_opcode_FX1E(opcode);
        }
        else if (nibble1 == 0x2 && nibble0 == 0x9)
        {
            execute_opcode_FX29(opcode, ram);
        }
        else if (nibble1 == 0x3 && nibble0 == 0x3)
        {
            execute_opcode_FX33(opcode, ram);
        }
        else if (nibble1 == 0x5 && nibble0 == 0x5)
        {
            execute_opcode_FX55(opcode, ram);
        }
        else if (nibble1 == 0x6 && nibble0 == 0x5)
        {
            execute_opcode_FX65(opcode, ram);
        }
        else
        {
            thoth::fatal("Invalid operation of type FXXX: %04x", opcode);
            std::abort();
        }
    }

    void cProcessor::execute_opcode_00E0(uint16_t opcode, cDisplay* display) & noexcept
    {
        // Clears screen.
        display->clear_frame();
    }

    void cProcessor::execute_opcode_00EE(uint16_t opcode, cRam* ram) & noexcept
    {
        // Return from function
        _program_counter = ram->pop_from_stack();
    }

    void cProcessor::execute_opcode_1NNN(uint16_t opcode) & noexcept
    {
        // Jumps to NNN.
        _program_counter = opcode & 0x0FFF;
    }

    void cProcessor::execute_opcode_2NNN(uint16_t opcode, cRam* ram) & noexcept
    {
        // Calls subroutine at NNN
        ram->push_to_stack(_program_counter);
        _program_counter = opcode & 0x0FFF;
    }

    void cProcessor::execute_opcode_3XNN(uint16_t opcode) & noexcept
    {
        // If vx == NN, skip next instruction
        if (_registers[get_nibble(opcode, 2)] == (opcode & 0xFF))
        {
            _program_counter += 2U;
        }
    }

    void cProcessor::execute_opcode_4XNN(uint16_t opcode) & noexcept
    {
        // If vx != NN, skip the next instruction
        if (_registers[get_nibble(opcode, 2)] != (opcode & 0xFF))
        {
            _program_counter += 2U;
        }
    }

    void cProcessor::execute_opcode_5XY0(uint16_t opcode) & noexcept
    {
        // Skip next instruction if Vx == Vy
        if (_registers[get_nibble(opcode, 1)] == _registers[get_nibble(opcode, 2)])
        {
            _program_counter += 2U;
        }
    }

    void cProcessor::execute_opcode_6XNN(uint16_t opcode) & noexcept
    {
        // Set VX to NN
        _registers[get_nibble(opcode, 2)] = (opcode & 0xFF);
    }

    void cProcessor::execute_opcode_7XNN(uint16_t opcode) & noexcept
    {
        // Adds NN to Vx (carry flag is not changed)
        _registers[get_nibble(opcode, 2)] += (opcode & 0xFF);
    }

    void cProcessor::execute_opcode_8XY0(uint16_t opcode) & noexcept
    {
        // Vx = Vy
        _registers[get_nibble(opcode, 2)] = _registers[get_nibble(opcode, 1)];
    }

    void cProcessor::execute_opcode_8XY1(uint16_t opcode) & noexcept
    {
        // Vx |= Vy
        _registers[get_nibble(opcode, 2)] |= _registers[get_nibble(opcode, 1)];
    }

    void cProcessor::execute_opcode_8XY2(uint16_t opcode) & noexcept
    {
        // Vx &= Vy
        _registers[get_nibble(opcode, 2)] &= _registers[get_nibble(opcode, 1)];
    }

    void cProcessor::execute_opcode_8XY3(uint16_t opcode) & noexcept
    {
        // Vx ^= Vy
        _registers[get_nibble(opcode, 2)] ^= _registers[get_nibble(opcode, 1)];
    }

    void cProcessor::execute_opcode_8XY4(uint16_t opcode) & noexcept
    {
        // Vx += Vy. Set VF to 1 if there is overflow, to 0 if not.
        size_t const  register_index_x {get_nibble(opcode, 2)};
        uint8_t const register_content_x {_registers[register_index_x]};
        uint8_t const sum {static_cast<uint8_t>(register_content_x + _registers[get_nibble(opcode, 1)])};
        _registers[register_index_x] = sum;
        _registers[15] = sum < register_content_x ? 1 : 0; // TODO: This overflow check does not work, does it?
    }

    void cProcessor::execute_opcode_8XY5(uint16_t opcode) & noexcept
    {
        // Vx -= Vy. Vf set to 0 if there is underflow, to 0 if not (VF set to 1 if Vx >= Vy)
        size_t const  register_index_x {get_nibble(opcode, 2)};
        uint8_t const register_content_x {_registers[register_index_x]};
        uint8_t const register_content_y {_registers[get_nibble(opcode, 1)]};
        _registers[register_index_x] = register_content_x - register_content_y;
        _registers[15] = register_content_x >= register_content_y ? 1 : 0;
    }

    void cProcessor::execute_opcode_8XY6(uint16_t opcode) & noexcept
    {
        // Vx >>= 1. Store least significand bit of Vx prior to shift to VF.
        size_t const  register_index {get_nibble(opcode, 2)};
        uint8_t const register_content {_registers[register_index]};
        _registers[register_index] = register_content >> 1;
        _registers[15] = register_content & 0x01;
    }

    void cProcessor::execute_opcode_8XY7(uint16_t opcode) & noexcept
    {
        // Vx = Vy - Vx. Vf set to 0 if Vy >= Vx, to 1 otherwise.
        size_t const  register_index_x {get_nibble(opcode, 2)};
        uint8_t const register_content_x {_registers[register_index_x]};
        uint8_t const register_content_y {_registers[get_nibble(opcode, 1)]};
        _registers[register_index_x] = register_content_y - register_content_x;
        _registers[15] = register_content_y >= register_content_x ? 1 : 0;
    }

    void cProcessor::execute_opcode_8XYE(uint16_t opcode) & noexcept
    {
        // Vx <<= 1. Set Vf to 1 if most significanf bit of Vx prior to shift was set, to 0 otherwise.
        size_t const  register_index {get_nibble(opcode, 2)};
        uint8_t const register_content {_registers[register_index]};
        _registers[register_index] = register_content << 1;
        _registers[15] = (register_content & 0x80) >> 7;
    }

    void cProcessor::execute_opcode_9XY0(uint16_t opcode) & noexcept
    {
        // Skip next instruction if Vx != Vy
        if (_registers[get_nibble(opcode, 1)] != _registers[get_nibble(opcode, 2)])
        {
            _program_counter += 2U;
        }
    }

    void cProcessor::execute_opcode_ANNN(uint16_t opcode) & noexcept
    {
        // I = NNN
        _register_i = opcode & 0x0FFF;
    }

    void cProcessor::execute_opcode_BNNN(uint16_t opcode) & noexcept
    {
        // PC = V0 + NNN
        _program_counter = static_cast<uint16_t>(_registers[0]) + (opcode & 0x0FFF);
    }

    void cProcessor::execute_opcode_CXNN(uint16_t opcode) & noexcept
    {
        // Vx = rand(0,255) & NN
        _registers[get_nibble(opcode, 2)] = static_cast<uint8_t>((rand() % 255)) & (opcode & 0x00FF);
    }

    void cProcessor::execute_opcode_DXYN(uint16_t opcode, cRam* ram, cDisplay* display) & noexcept
    {
        // draw(vx, vy, N). Draw a sprite at coordinate Vx, Vy
        // that has a width of 8 pixels and a height of N pixels.
        // Each row of 8 pixels is read as bit coded starting from memory location I.
        // I does no change.
        // Set VF to 1 if any bits are flipped from set to unset when drawing, 0 if that does not happen.
        uint8_t const sprite_start_x {static_cast<uint8_t>(_registers[get_nibble(opcode, 2)] % display->get_width())};
        uint8_t const sprite_start_y {static_cast<uint8_t>(_registers[get_nibble(opcode, 1)] % display->get_height())};

        bool flipped_any_bit {false};

        for (uint8_t row {0}; row < get_nibble(opcode, 0); row++)
        {
            bool row_flipped_any_bit {false};

            display->draw_byte(sprite_start_x, sprite_start_y + row, ram->read(_register_i + row), &row_flipped_any_bit);
            flipped_any_bit |= row_flipped_any_bit;
        }

        _registers[15] = flipped_any_bit ? 1U : 0U;
    }

    void cProcessor::execute_opcode_EX9E(uint16_t opcode, cKeyboard* keyboard) & noexcept
    {
        // Skip next instruction if key stored in Vx (consider only lowest nibble (half-bit)) is pressed.
        if (keyboard->is_key_pressed(_registers[get_nibble(opcode, 2)]))
        {
            _program_counter += 2;
        }
    }

    void cProcessor::execute_opcode_EXA1(uint16_t opcode, cKeyboard* keyboard) & noexcept
    {
        // Skip next instruction if key stored in Vx (consider only lowest nibble (half-bit)) is NOT pressed.
        if (!keyboard->is_key_pressed(_registers[get_nibble(opcode, 2)]))
        {
            _program_counter += 2;
        }
    }

    void cProcessor::execute_opcode_FX07(uint16_t opcode, cTimer* delay_timer) & noexcept
    {
        // Sets vx to the current value of the delay timer
        _registers[get_nibble(opcode, 2)] = delay_timer->get_time();
    }

    void cProcessor::execute_opcode_FX0A(uint16_t opcode, cKeyboard* keyboard) & noexcept
    {
        // A key press is awaited, then stored in Vx
        // If no key is pressed we decrement program counter as to execute this instruction again.
        // But sound and delay timers keep processing.
        if (int8_t const pressed_key {keyboard->await_key_press()}; -1 == pressed_key)
        {
            _program_counter -= 2;
        }
        else
        {
            _registers[get_nibble(opcode, 2)] = static_cast<uint8_t>(pressed_key);
        }
    }

    void cProcessor::execute_opcode_FX15(uint16_t opcode, cTimer* delay_timer) & noexcept
    {
        // Sets delay timer to Vx
        delay_timer->set_time(_registers[get_nibble(opcode, 2)]);
    }

    void cProcessor::execute_opcode_FX18(uint16_t opcode, cTimer* sound_timer) & noexcept
    {
        // Sets sound timer to Vx
        sound_timer->set_time(_registers[get_nibble(opcode, 2)]);
    }

    void cProcessor::execute_opcode_FX1E(uint16_t opcode) & noexcept
    {
        // I += Vx. Vf is not affected.
        _register_i += _registers[get_nibble(opcode, 2)];

        // TODO: Surely this can be simplified with the modulo operator
        if (_register_i > RAM_SIZE)
        {
            _register_i -= (RAM_SIZE + 1);
        }

        if (_register_i < 0U)
        {
            _register_i += (RAM_SIZE + 1);
        }
    }

    void cProcessor::execute_opcode_FX29(uint16_t opcode, cRam* ram) & noexcept
    {
        // In a particular memory location are sprites for the characters 0-9 and A-F, in 4x5 font.
        // This instruction read the lowest nibble (half bit) of Vx and then sets I to the location
        // in memory where you can find the sprite for said character.
        _register_i = ram->get_font_char_position(get_nibble(opcode, 0));
    }

    void cProcessor::execute_opcode_FX33(uint16_t opcode, cRam* ram) & noexcept
    {
        // Takes the number in Vx. Reads it as in decimal. Like if Vx is OxE7 it considers it 231.
        // Then stores 231 in I, like it stores 2 in I, 3 in I+1 and 1 in I+2.
        // This is called binary coded decimal.
        uint8_t register_content {_registers[get_nibble(opcode, 2)]};
        ram->write(_register_i, register_content / 100);
        ram->write(_register_i + 1, (register_content % 100) / 10);
        ram->write(_register_i + 2, register_content % 10);
    }

    void cProcessor::execute_opcode_FX55(uint16_t opcode, cRam* ram) & noexcept
    {
        // Stores from V0 to Vx (including Vx) in memory, starting at adress I.
        // I is not modified.
        for (size_t i {0U}; i <= get_nibble(opcode, 2); i++)
        {
            ram->write(_register_i + i, _registers[i]);
        }
    }

    void cProcessor::execute_opcode_FX65(uint16_t opcode, cRam* ram) & noexcept
    {
        // Fills V0 to Vx (including Vx) with values from memory, starting at adress I.
        // I is not modified.
        for (size_t i {0U}; i <= get_nibble(opcode, 2); i++)
        {
            _registers[i] = ram->read(_register_i + i);
        }
    }
}
