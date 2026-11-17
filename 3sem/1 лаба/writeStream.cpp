//ошибки
#include "Stream.hpp"
#include <string>
#include "writeStream.hpp"
#include "error.hpp"

WriteStream::WriteStream(size_t bufferSize):
    Stream(bufferSize), countWrite(0) {}

void WriteStream::open(const std::string& filename){
    if (isOpen){
        return ;
    }
    file = fopen(filename.c_str(), "a+b");
    if (!file){
        throw Error(Error::FileCreateError);
    }
    fileName = filename;
    isOpen = true;
    countWrite = 0;
}


void WriteStream::writeChar(char c){
    if (!isOpen){
        throw Error(Error::FileOpenError);
    }
    if (fputc(c, file) == EOF){
        throw Error(Error::FileWriteError);
    }
    countWrite++;
    fflush(file);
}

void WriteStream::writeWord(const std::string& word){
    if (!isOpen){
        throw Error(Error::FileOpenError);
    }
    for (char c : word){
        writeChar(c);
    }
    writeChar(' ');
}

void WriteStream::writeLine(const std::string& line){
    if(!isOpen){
        throw Error(Error::FileOpenError);
    }
    for (char c : line){
        writeChar(c);
    }
    writeChar('\n');
}

size_t WriteStream::getCountWrite() const{
    return countWrite;
}

void WriteStream::readBuffer() {
    throw Error(Error::BufferError);
}