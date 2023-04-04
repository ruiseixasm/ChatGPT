#include <iostream>

enum class ErrorCode {
    SUCCESS = 0,
    FILE_NOT_FOUND = 1,
    ACCESS_DENIED = 2,
    INVALID_ARGUMENT = 3,
    UNKNOWN_ERROR = 4
};

void processFile(const std::string& filename) {
    // some code to process file
    bool fileFound = false;
    bool accessGranted = true;
    if (!fileFound) {
        throw ErrorCode::FILE_NOT_FOUND;
    }
    if (!accessGranted) {
        throw ErrorCode::ACCESS_DENIED;
    }
}

int main() {
    try {
        processFile("test.txt");
    }
    catch (const ErrorCode& error) {
        switch (error) {
        case ErrorCode::FILE_NOT_FOUND:
            std::cerr << "Error: file not found." << std::endl;
            break;
        case ErrorCode::ACCESS_DENIED:
            std::cerr << "Error: access denied." << std::endl;
            break;
        case ErrorCode::INVALID_ARGUMENT:
            std::cerr << "Error: invalid argument." << std::endl;
            break;
        default:
            std::cerr << "Unknown error." << std::endl;
            break;
        }
        return static_cast<int>(error);
    }
    return static_cast<int>(ErrorCode::SUCCESS);
}
