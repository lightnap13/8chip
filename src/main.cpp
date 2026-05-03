#include "display.hpp"
#include "keyboard.hpp"
#include "log.hpp"
#include "processor.hpp"
#include "ram.hpp"
#include "timer.hpp"

#include <cstdlib>
#include <iostream>
#include <unistd.h>

constexpr int64_t MAX_INSTRUCTIONS = 20'000'000;
constexpr double  TIMER_PERIOD_MS {1000.0f / 60.0f};
constexpr double  INSTRUCTION_PERIOD_S {1.0f / chip8::INSTR_PER_SEC};

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        thoth::fatal("Expected 1 argument, %d were provided ", argc - 1);
        return EXIT_FAILURE;
    }

    std::srand(0); // TODO: Use actual random.

    chip8::cRam ram {chip8::RAM_SIZE, chip8::PROGRAM_START_LOCATION};
    if (ram.load_rom(argv[1]) != 0)
    {
        thoth::fatal("Could load rom: %s", argv[1]);
        return EXIT_FAILURE;
    }

    chip8::cDisplay   display {chip8::DISPLAY_HEIGHT, chip8::DISPLAY_WIDTH};
    chip8::cKeyboard  keyboard;
    chip8::cTimer     delay_timer {TIMER_PERIOD_MS};
    chip8::cTimer     sound_timer {TIMER_PERIOD_MS};
    chip8::cProcessor processor {chip8::PROGRAM_START_LOCATION, chip8::REGISTER_COUNT};

    for (int64_t instruction_count {0L}; instruction_count < MAX_INSTRUCTIONS; instruction_count++)
    {
        display.draw_frame();
        thoth::info("Instruction number %d\n", instruction_count);
        processor.execute_next_instruction(&ram, &display, &keyboard, &delay_timer, &sound_timer);

        if (sound_timer.get_time() != 0)
        {
            // TODO: Make beep timer actually make a sound
            std::cout << "BEEEEP!\n";
        }

        sleep(INSTRUCTION_PERIOD_S);
    }

    return EXIT_SUCCESS;
}
