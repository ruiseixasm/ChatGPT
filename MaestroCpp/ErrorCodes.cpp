#include "ErrorCodes.h"

const char* GetErrorCodeString(const ErrorCode& error_code) {
    // All strings directly typed in the code remain saved in the data segment (RAM).
    switch (error_code) {
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