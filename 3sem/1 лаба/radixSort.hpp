#ifndef RADIXSORT_HPP
#define RADIXSORT_HPP

#include "readStream.hpp"
#include "writeStream.hpp"
#include "error.hpp"
#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <ctime>
#include <cstdlib>

class RadixSort {
public:
    void radixSort(std::vector<int>& arr);
    void sortFromFile(const std::string& inputFilename, const std::string& outputFilename);
    void sortFromConsole(const std::string& outputFilename);
    void sortRandomFile(const std::string& filename, size_t count, int minVal, int maxVal);
    
    
    
    void sortFromFileString(const std::string& inputFilename, const std::string& outputFilename);
    void sortFromConsoleString(const std::string& outputFilename);
    void sortRandomFileString(const std::string& filename, size_t count, int minVal, int maxVal);
    void radixSortString(std::vector<std::string>& words);
};


#endif