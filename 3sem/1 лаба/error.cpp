#include "error.hpp"
#include <stdio.h>
#include <iostream>

void errorDetection(Error error){
    if (error == Error::OK){
        return;
    }
    else{
        if (error == Error::FileOpenError){
            std::cout << "Error: file is close";
        }
        if (error == Error::FileCreateError){
            std::cout << "File is not create";
        }
        if (error == Error::FileReadError){
            std::cout << "Error: can't read file";
        }
        if (error == Error::FileWriteError){
            std::cout << "Error: can't write";
        }
        if (error == Error::BufferSizeError){
            std::cout << "Error: buffer size can not be negetive";
        }
        if (error == Error::EndOfFileError){
            std::cout << "Error: it is end of file";
        }
        if (error == Error::BufferError){
            std::cout << "Error smth with buffer";
        }
        if (error == Error::FileCloseError){
            std::cout << "Error: file is close";
        }
        if (error == Error::EmptyValueError){
            std::cout << "Error: enter value to sort";
        }


    }
}
/*
enum class Error{
    OK = 0;
    FileOpenError = 1;
    FileCreateError = 2;
    FileReadError = 3;
    FileWriteError = 4;
    FileCloseError = 5;
    BufferError = 6;
    EndOfFileError = 7;
    BufferSizeError = 8;
    
}*/