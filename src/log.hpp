#ifndef CHIP8_SRC_LOGHPP
#define CHIP8_SRC_LOGHPP

namespace log
{
    enum eLevel
    {
        trace = 0,
        debug,
        info,
        warning,
        error,
        critical,
        fatal,
    };

    void log(eLevel level, const char* message);
}

#endif // CHIP8_SRC_LOGHPP
