#ifndef WRITESTREAM_HPP
#define WRITESTREAM_HPP
#include "Stream.hpp"
#include <string>

class WriteStream: public Stream {
    private: 
        size_t countWrite;
    
    protected:
        void readBuffer();
    public:
        WriteStream(size_t bufferSize = bufferSizeConst);
        void open(const std::string& filename);

        virtual void writeChar(char c);
        virtual void writeWord(const std::string& word);
        virtual void writeLine(const std::string& line);

        virtual size_t getCountWrite() const;
};
#endif 