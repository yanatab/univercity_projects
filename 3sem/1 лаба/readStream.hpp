#ifndef READSTREAM_HPP
#define READSTREAM_HPP
#include "Stream.hpp"
#include <string>


class ReadStream : public Stream{
    private: 
        size_t filePosition;
        bool endOfFile;
        std::string miniBuffer;
    
    protected:
        void readBuffer();
    
    public:
        ReadStream(size_t bufferSize = bufferSizeConst);
        void open(const std::string& filename);

        virtual char readChar();
        virtual std::string readWord();
        virtual std::string readLine();
        virtual void Seek(size_t position);

        virtual bool isEndOfFile() const;
        virtual size_t getPosition() const;

};
#endif 