#ifndef CHIP8_SRC_LOGHPP
#define CHIP8_SRC_LOGHPP

namespace thoth
{

    enum class eLevel
    {
        trace = 0, // Information that helps a developer troubleshoot problems.
        debug,     // Information that helps an user troubleshoot problems.
        info,      // Porgram is working normally.
        warning,   // Noticed something strange but operation can proceed.
        error,     // Aborting current operation.
        critical,  // Program has not crashed yet but statuus is crititcal.
        fatal,     // Program is crashing.
    };

    // Possible config struct.
    // struct sLogConfig
    //{
    //    bool        logs_active {true};
    //    bool        output_to_stdout {true};
    //    bool        output_to_file {false};
    //    std::string path_file_output {""};
    //    eLevel      log_level {eLevel::info};
    //    bool        color_output {true};
    //    bool        print_date {true};
    //    bool        print_log_level {true};
    //    bool        print_program_name {false};
    //};

    // For now, we hide this function.
    // Who knows if we will need it some day.
    // void log(eLevel level, const char* message, ...);
    void trace(const char* message, ...) noexcept;
    void debug(const char* message, ...) noexcept;
    void info(const char* message, ...) noexcept;
    void warning(const char* message, ...) noexcept;
    void error(const char* message, ...) noexcept;
    void critical(const char* message, ...) noexcept;
    void fatal(const char* message, ...) noexcept;
}

#endif // CHIP8_SRC_LOGHPP
