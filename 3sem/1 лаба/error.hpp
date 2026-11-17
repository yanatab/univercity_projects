#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
#include <stdexcept>
#include <iostream>

enum class Error{
    OK = 0,
    FileOpenError = 1,
    FileCreateError = 2,
    FileReadError = 3,
    FileWriteError = 4,
    FileCloseError = 5,
    BufferError = 6,
    EndOfFileError = 7,
    BufferSizeError = 8,
    EmptyValueError = 9,
};

void errorDetection(Error error);
#endif