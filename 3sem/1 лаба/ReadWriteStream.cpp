
#include "ReadWriteStream.hpp"
#include "error.hpp"

ReadWriteStream::ReadWriteStream(size_t bufferSize)
    : reader(bufferSize), writer(bufferSize), ReadingMode(true){}

void ReadWriteStream::open(const std::string& filename) {
    FILE* testFile = fopen(filename.c_str(), "r+b");
    if (!testFile) {
        testFile = fopen(filename.c_str(), "w+b");
    }
    if (testFile) {
        fclose(testFile);
    }
    reader.open(filename);
    writer.open(filename);
}

void ReadWriteStream::close() {
    reader.close();
    writer.close();
}