#include "display.hpp"
#include "keyboard.hpp"
#include "log.hpp"
#include "processor.hpp"
#include "ram.hpp"
#include "timer.hpp"

#include <cstdlib>
#include <iostream>
#include <unistd.h>

constexpr int32_t MAX_INSTRUCTIONS = 20'000'000;

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        thoth::fatal("Expected 1 argument, %d were provided ", argc);
        return EXIT_FAILURE;
    }

    std::srand(0); // TODO: Use actual random.

    chip8::cRam ram {chip8::RAM_SIZE, chip8::PROGRAM_START_LOCATION};

    if (ram.load_rom(argv[1]) != 0)
    {
        thoth::fatal("Could load rom: %s", argv[1]);
        return EXIT_FAILURE;
    }

    ram.print();

    chip8::cDisplay display {chip8::DISPLAY_HEIGHT, chip8::DISPLAY_WIDTH};
    // display.clear_pixels();

    chip8::cKeyboard keyboard;

    constexpr float timer_period_ms {1000.0f / 60.0f};
    chip8::cTimer   delay_timer {timer_period_ms};
    chip8::cTimer   sound_timer {timer_period_ms};

    chip8::cProcessor processor {chip8::PROGRAM_START_LOCATION, chip8::REGISTER_COUNT};

    constexpr float instruction_period_s = (1.0f / chip8::INSTR_PER_SEC);

    for (int i = 0; i < MAX_INSTRUCTIONS; i++)
    {
        display.draw_frame();
        thoth::info("Frame number %d\n", i);
        processor.execute_next_instruction(&ram, &display, &keyboard, &delay_timer, &sound_timer);

        if (sound_timer.get_time() == 0)
        {
            // TODO: Make beep timer actually make a sound
            std::cout << "BEEEEP!\n";
        }

        // ram.print();
        // delay_timer.print();
        sleep(instruction_period_s);
    }

    return EXIT_SUCCESS;
}
