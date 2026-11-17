#ifndef ERRORS_H
#define ERRORS_H

#include <iostream>

enum class Error {
    OK,
    INVALID_INDEX,
    EMPTY_TREE,
    INVALID_ARGUMENT,
    NULL_NODE,
    KEY_NOT_FOUND,
    INVALID_RANGE,
    CACHE_FULL,
    OUT_OF_MEMORY
};

inline void errorsDetection(Error error) {
    if (error == Error::OK) {
        return;
    }
    else{
        if(error == Error::INVALID_INDEX){
            std::cout << "Invalid index.\n";
        }
        if(error == Error::EMPTY_TREE){
            std::cout << "Empty tree.\n";
        }
        if(error == Error::INVALID_ARGUMENT){
            std::cout << "Invalid argument.\n";
        }
        if(error == Error::NULL_NODE){
            std::cout << "Null node.\n";
        }
        if(error == Error::KEY_NOT_FOUND){
            std::cout << "Key not found.\n";
        }
        if(error == Error::INVALID_RANGE){
            std::cout << "Invalid range.\n";
        }
        if(error == Error::CACHE_FULL){
            std::cout << "Cache is full.\n";
        }
        if(error == Error::OUT_OF_MEMORY){
            std::cout << "Out of memory.\n";
        }
    }
}

#endif