#include "log.hpp"

#include <cstdarg>
#include <cstdio>
#include <unordered_map>

namespace thoth
{
    const std::unordered_map<eLevel, const char*> level_to_str {{eLevel::trace, "[TRACE]"},
                                                                {eLevel::debug, "[DEBUG]"},
                                                                {eLevel::info, "[INFO_]"},
                                                                {eLevel::warning, "[WARN_]"},
                                                                {eLevel::error, "[ERROR]"},
                                                                {eLevel::critical, "[CRTCL]"},
                                                                {eLevel::fatal, "[FATAL]"}};

    void log(eLevel level, const char* message, ...);
    void log(eLevel level, const char* message, std::va_list arguments);

    void debug(const char* message, ...)
    {
        std::va_list argp;
        va_start(argp, message);
        log(eLevel::debug, message, argp);
        va_end(argp);
    }

    void info(const char* message, ...)
    {
        std::va_list argp;
        va_start(argp, message);
        log(eLevel::info, message, argp);
        va_end(argp);
    }

    void warning(const char* message, ...)
    {
        std::va_list argp;
        va_start(argp, message);
        log(eLevel::warning, message, argp);
        va_end(argp);
    }

    void error(const char* message, ...)
    {
        std::va_list argp;
        va_start(argp, message);
        log(eLevel::error, message, argp);
        va_end(argp);
    }

    void log(eLevel level, const char* message, ...)
    {
        std::va_list argp;
        va_start(argp, message);
        log(level, message, argp);
        va_end(argp);
    }

    // TODO: Variadic arguments not getting pritned.
    void log(eLevel level, const char* message, std::va_list arguments)
    {
        std::printf("%s ", level_to_str.at(level));
        std::vfprintf(stdout, message, arguments);
        std::printf("\n");
    }

}
