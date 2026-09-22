#ifndef DATA_PROCESSING_APPLICATION_UTILS_LOGGER_H
#define DATA_PROCESSING_APPLICATION_UTILS_LOGGER_H

#include <iostream>
#include <string>

namespace data_processing::utils {

class Logger {
public:
    static void logInfo(const std::string& message){
        std::cout << "INFO: " << message << std::endl;
    }

    static void logError(const std::string& message){
        std::cerr << "ERROR: " << message << std::endl;
    }

    static void logDebug(const std::string& message){
        std::cout << "DEBUG: " << message << std::endl;
    }

    static void logWarning(const std::string& message){
        std::cout << "WARNING: " << message << std::endl;
    }
};

} // namespace data_processing::utils

#endif // DATA_PROCESSING_APPLICATION_UTILS_LOGGER_H