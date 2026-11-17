//ошибочкиошибочкиошибочки(вроде сделано)
#include "Stream.hpp"
#include "error.hpp"

Stream::Stream(size_t bufferSize) 
    : charInd(0), isOpen(false), charCount(0), bufferSize(bufferSize), file(nullptr) {
}
Stream::~Stream(){
    if (isOpen){
        close();
    }
}
 
void Stream::close(){
    if(isOpen){
        fclose(file);
        file = nullptr;
    }
    isOpen = false;
    charInd = 0;
    charCount = 0;
    std::fill(buffer.begin(), buffer.end(), 0);

}

void Stream::resizeBuffer(size_t newSize){
    if (isOpen){
        throw Error(Error::FileOpenError);
    }
    if (newSize <= 0){
        throw Error(Error::BufferSizeError);
    }
    bufferSize = newSize;
    buffer.resize(bufferSize);
    std::fill(buffer.begin(), buffer.end(), 0);
    charInd = 0;
}

void Stream::initBuffer(){
    if (isOpen){
        buffer.resize(bufferSize);
        std::fill(buffer.begin(), buffer.end(), 0);
    }
}
