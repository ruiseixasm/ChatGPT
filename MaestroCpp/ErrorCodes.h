#ifndef ERROR_CODES_H
#define ERROR_CODES_H

#include <iostream>

enum class ErrorCode {
    Success,
    InvalidInput,
    FileNotFound,
    UnknownError
};

const char* GetErrorCodeString(const ErrorCode& error_code);

#endif /* ERROR_CODES_H */