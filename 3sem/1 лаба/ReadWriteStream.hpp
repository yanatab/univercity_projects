#ifndef READWRITESTREAM_HPP
#define READWRITESTREAM_HPP
#include "readStream.hpp"
#include "writeStream.hpp"
#include <string>

class ReadWriteStream {
private:
    ReadStream reader;
    WriteStream writer;
    bool ReadingMode;
    
public:
    ReadWriteStream(size_t bufferSize = 200);
    void open(const std::string& filename);
    void close();

    void switchToRead() { ReadingMode = true;}
    void switchToWrite() { ReadingMode = false;}

    bool isReadingMode() const {return ReadingMode;}
    bool isWriteMode() const {return !ReadingMode;}
    

    char readChar() { return reader.readChar(); }
    std::string readWord() { return reader.readWord(); }
    std::string readLine() { return reader.readLine(); }
    bool isEndOfFile() const { return reader.isEndOfFile(); }
    size_t getPosition() const { return reader.getPosition(); }
    size_t getCharCount() const { return reader.getCharCount(); }
    void Seek(size_t position) {return reader.Seek(position); }
    
    
    void writeChar(char c) { writer.writeChar(c); }
    void writeWord(const std::string& word) { writer.writeWord(word); }
    void writeLine(const std::string& line) { writer.writeLine(line); }
    size_t getCountWrite() const { return writer.getCountWrite(); }
    
    bool streamOpen() const { return reader.streamOpen(); }
    std::string getFileName() {return reader.getFileName();}
};
#endif 
