//ошибки (норм)
#include "readStream.hpp"
#include "error.hpp"

ReadStream::ReadStream(size_t bufferSize)
    : Stream(bufferSize), filePosition(0), endOfFile(false){}

void ReadStream::open(const std::string& filename){
    if (isOpen){
        return ;
    }
    file = fopen(filename.c_str(), "rb");
    if(!file){
        throw Error(Error::FileOpenError);
    }
    fileName = filename;
    isOpen = true;
    filePosition = 0;
    endOfFile = false;
    charInd = 0;
    charCount = 0;
    initBuffer();
    readBuffer();

}

void ReadStream::readBuffer() {
    if (!isOpen){
        throw Error(Error::FileOpenError);
    }
    if (!file){
        throw Error(Error::FileCreateError);
    }
    size_t bytesRead = fread(buffer.data(), 1, buffer.size(), file);
    charInd = 0;
}

char ReadStream::readChar(){
    if (!isOpen){
        throw Error(Error::FileOpenError);
    }
    if (endOfFile){
        throw Error(Error::EndOfFileError);
    }
    if (charInd >= buffer.size()){
        readBuffer();
    }
    
    char c = buffer[charInd++];
    if (c != ' ' && c != ',' && c != '.' && c != ':' && c != ';' && c != '\n') {
        miniBuffer.push_back(c);
    } else {
        miniBuffer.clear();
    }
    charCount++;
    filePosition++;
    return c;
}

std::string ReadStream::readWord(){
    if (!isOpen) {
        throw Error(Error::FileOpenError);
    }
    if (endOfFile){
        throw Error(Error::EndOfFileError);
    }
    std::string currentWord = miniBuffer;
    while (true){
        char c = readChar();
        if (c == ' ' || c == ',' || c == '.' || c == ':' || c == ';' || c == '\n') {
                break;
        }
        currentWord = miniBuffer;
    }
    miniBuffer.clear();
    return currentWord;
}

std::string ReadStream::readLine(){ /*макс размер строки*/
    if (!isOpen){
        throw Error(Error::FileOpenError);
    }
    std::string line = miniBuffer;
    
    while(line.length() < bufferSize){
        if (charInd >= buffer.size()){
            readBuffer();
        }
        if (endOfFile){
            throw Error(Error::EndOfFileError);
            break;
        }
        char c = readChar();
        if (c == '\n'){
            break;
        }
        line += c;
    }
    return line;
}

void ReadStream::Seek(size_t position){
    if (!isOpen){
        throw Error(Error::FileOpenError);
    }
    fseek(file, position-1, SEEK_SET);
    filePosition = position;
    charInd = 0;
    endOfFile = false;
    miniBuffer.clear();

    readBuffer();
}

bool ReadStream::isEndOfFile() const{
    if (!file) {
        throw Error(Error::FileCreateError);
    }
    if (!isOpen){
        throw Error(Error::FileOpenError);
    }
    return feof(file) != 0;
}

size_t ReadStream::getPosition() const{
    return filePosition;
}

