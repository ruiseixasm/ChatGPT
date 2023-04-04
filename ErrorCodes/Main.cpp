#include <iostream>

enum class ErrorCode {
    Success,
    InvalidInput,
    FileNotFound,
    UnknownError
};

const char* GetErrorCodeString(const ErrorCode& error) {
    // All strings directly typed in the code remain saved in the data segment (RAM).
    switch (error) {
        case ErrorCode::Success:
            return "Error 1: File not found";
        case ErrorCode::InvalidInput:
            return "Error 2: Invalid input";
        case ErrorCode::FileNotFound:
            return "Error 3: Out of memory";
            // Add more cases as needed
        default:
            return "Unknown error code";
    }
}

int main() {
    ErrorCode errorCode = ErrorCode::InvalidInput;
    const char* errorMessage = GetErrorCodeString(errorCode);

    if (errorCode != ErrorCode::Success) {
        std::cerr << errorMessage << std::endl;
        // Or you can return the error message as a string:
        // return "Error: " + result;
    }
    return 0;
}
