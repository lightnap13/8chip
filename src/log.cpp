#include "log.hpp"

#include <cstdio>
#include <unordered_map>

namespace log
{
    const std::unordered_map<eLevel, const char*> level_to_str {{eLevel::trace, "[TRACE]"},
                                                                {eLevel::debug, "[DEBUG]"},
                                                                {eLevel::info, "[INFO]"},
                                                                {eLevel::warning, "[ WARN]"},
                                                                {eLevel::error, "[ERROR]"},
                                                                {eLevel::critical, "[ CRIT]"},
                                                                {eLevel::fatal, "[FATAL]"}};

    void log(eLevel level, const char* message)
    {
        std::printf("%s %s", level_to_str.at(level), message);
    }
}
