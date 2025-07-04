#include "display.hpp"
#include "keyboard.hpp"
#include "processor.hpp"
#include "ram.hpp"
#include "timer.hpp"

#include <cstdlib>
#include <iostream>
#include <unistd.h>

int main()
{

    std::srand(0); // TODO: Use actual random.

    chip8::cRam ram {chip8::RAM_SIZE, chip8::PROGRAM_START_LOCATION};

    if (ram.load_rom("../data/octojam6title.ch8") != 0)
    {
        return EXIT_FAILURE;
    }

    ram.print();

    chip8::cDisplay display {chip8::DISPLAY_HEIGHT, chip8::DISPLAY_WIDTH};
    // display.clear_pixels();

    chip8::cKeyboard keyboard;

    chip8::cTimer delay_timer {chip8::cTimer::eType::delay};
    chip8::cTimer sound_timer {chip8::cTimer::eType::sound};

    chip8::cProcessor processor {chip8::PROGRAM_START_LOCATION, chip8::REGISTER_COUNT};

    for (int i = 0; i < 20; i++)
    {

        // display.draw_frame();
        std::cout << "[INFO] Frame number " << i << std::endl;
        std::cout << "\n\n";
        processor.execute_next_instruction(&ram, &display, &keyboard, &delay_timer, &sound_timer);

        // ram.print();
        sleep(1);
    }

    return EXIT_SUCCESS;
}
