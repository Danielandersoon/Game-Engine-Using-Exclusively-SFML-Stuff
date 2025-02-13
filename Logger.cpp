#include "Logger.h"

void GUESS::core::Logger::log(Level level, const std::string& message)
{
    std::string prefix;
    switch (level) {
    case INFO: prefix = "[INFO] "; break;
    case WARNING: prefix = "[WARNING] "; break;
    case ERROR: prefix = "[ERROR] "; break;
    case DEBUG: prefix = "[DEBUG] "; break;
    }

    std::cout << prefix << message << std::endl;
    logToFile(prefix + message);
}

void GUESS::core::Logger::logToFile(const std::string& message)
{
    static std::ofstream logFile("engine.log", std::ios::app);
    logFile << message << std::endl;
}
