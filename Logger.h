#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <fstream>

namespace GUESS::core {
    class Logger {
    public:
        enum Level { INFO, WARNING, ERROR, DEBUG };
        
        static void log(Level level, const std::string& message);
        
    private:
        static void logToFile(const std::string& message);
    };
}

#endif