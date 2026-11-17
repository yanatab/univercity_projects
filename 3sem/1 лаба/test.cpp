#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include "readStream.hpp"
#include "writeStream.hpp"
#include "ReadWriteStream.hpp"
#include "radixSort.hpp"
#include "error.hpp"

void create_test_file(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
    file.close();
}

void cleanup_file(const std::string& filename) {
    remove(filename.c_str());
}

void test_read_stream_basic() {
    std::cout << "Test read_stream_basic: ";
    
    create_test_file("test_basic.txt", "Hello World 123");
    
    ReadStream stream;
    
    stream.open("test_basic.txt");
    assert(stream.streamOpen() == true);
    assert(stream.getFileName() == "test_basic.txt");
    
    char c1 = stream.readChar();
    char c2 = stream.readChar();
    assert(c1 == 'H');
    assert(c2 == 'e');
    
    assert(stream.getPosition() > 0);
    
    stream.close();
    cleanup_file("test_basic.txt");
    std::cout << "Passed\n";
}

void test_read_stream_operations() {
    std::cout << "Test read_stream_operations: ";
    
    create_test_file("test_ops.txt", "First Second Third");
    
    ReadStream stream;
    stream.open("test_ops.txt");
    
    std::string word1 = stream.readWord();
    std::string word2 = stream.readWord(); 
    std::string word3 = stream.readWord();
    
    assert(!word1.empty());
    assert(!word2.empty());
    assert(!word3.empty());
    
    stream.close();
    cleanup_file("test_ops.txt");
    std::cout << "Passed\n";
}

void test_write_stream_basic() {
    std::cout << "Test write_stream_basic: ";
    
    WriteStream stream;
    stream.open("test_write_basic.txt");
    assert(stream.streamOpen() == true);
    
    stream.writeChar('A');
    stream.writeWord("test");
    stream.writeLine("line");
    
    assert(stream.getCountWrite() > 0);
    
    stream.close();
    
    std::ifstream check("test_write_basic.txt");
    assert(check.good());
    check.close();
    
    cleanup_file("test_write_basic.txt");
    std::cout << "Passed\n";
}



void test_error_conditions() {
    std::cout << "Test error_conditions: ";
    
    ReadStream read_stream;
    WriteStream write_stream;
    
  
    try {
        read_stream.readChar();
        assert(false); 
    } catch (const Error& e) {
    
        assert(e == Error::FileOpenError);
    }
    
   
    try {
        read_stream.open("non_existent_file.xyz");
        assert(false);   
    } catch (const Error& e) {
       
        assert(e == Error::FileOpenError);
    }
    
    std::cout << "Passed\n";
}

void test_empty_array_sort() {
    std::cout << "Test empty_array_sort: ";
    
    RadixSort sorter;
    std::vector<int> empty;
    
    try {
        sorter.radixSort(empty);
        assert(false); 
    } catch (const Error& e) {
        assert(e == Error::EmptyValueError);
    }
    
    std::cout << "Passed\n";
}

void test_read_write_stream_basic() {
    std::cout << "Test read_write_stream_basic: ";
    
    create_test_file("test_rw.txt", "initial data");
    
    ReadWriteStream stream;
    stream.open("test_rw.txt");
    
    assert(stream.isReadingMode() == true);
    stream.switchToWrite();
    assert(stream.isWriteMode() == true);
    stream.switchToRead();
    assert(stream.isReadingMode() == true);
    
    assert(stream.streamOpen() == true);
    
    stream.close();
    cleanup_file("test_rw.txt");
    std::cout << "Passed\n";
}

void run_core_tests() {
    std::cout << "Running core tests...\n\n";
    
    test_read_stream_basic();
    test_read_stream_operations();
    test_write_stream_basic();
    test_error_conditions();
    test_empty_array_sort();
    test_read_write_stream_basic();
    
    std::cout << "\nAll core tests passed!\n";
}

int main() {
    try {
        run_core_tests();
        return 0;
    } catch (const std::exception& e) {
        std::cout << "Test failed with exception: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cout << "Test failed with unknown exception\n";
        return 1;
    }
}