#ifndef STREAM_HPP
#define STREAM_HPP
#include <string>
#include <vector>
#include <cstdio>


class Stream {
    protected: 
        static const size_t bufferSizeConst = 200;
        bool isOpen;
        std::vector<char> buffer;
        size_t charInd;
        std::string fileName;
        
        size_t charCount;
        size_t bufferSize;
        FILE* file;

        virtual void readBuffer() = 0;

    public:
        Stream(size_t bufferSize = bufferSizeConst);
        virtual ~Stream();

        virtual void close();

        bool streamOpen() const{
            return isOpen;
        }

        size_t getCharCount() const {
            return charCount;
        }

        size_t getCharInd() const {
            return charInd;
        }

        size_t getBufferSize() const{
            return buffer.size();
        }

        std::string getFileName() const {
            return fileName;
        }

        void resizeBuffer(size_t newSize);
        
    
    protected:
        void initBuffer();
        
};
#endif 