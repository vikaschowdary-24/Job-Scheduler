#include "Logger.h"
#include <iostream>

void Logger::log(LogLevel level, const std::string& message) {
    switch (level) {
        case LogLevel::INFO:
            std::cout << "[INFO] " << message << "\n";
            break;
        case LogLevel::WARNING:
            std::cout << "[WARNING] " << message << "\n";
            break;
        case LogLevel::ERROR:
            std::cerr << "[ERROR] " << message << "\n";
            break;
    }
}