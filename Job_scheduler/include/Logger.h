#ifndef LOGGER_H
#define LOGGER_H

#include <string>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    // Static method allowing global logging without instantiating a Logger object
    static void log(LogLevel level, const std::string& message);
};

#endif // LOGGER_H