#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP

#include "IDictionary.hpp"
#include "errors.h"
#include <vector>
#include <functional>
#include <string>
#include <iostream>
#include <algorithm>
#include <climits>

template<typename T>
class HashHistogram {
private:
    std::function<int(T)> getValue;
    int minValue;
    int maxValue;
    int binSize;
    int numBins;
    int maxBarWidth;

public:
    HashHistogram(std::function<int(T)> valueFunc, int minVal, int maxVal, int bins, int maxWidth = 50)
        : getValue(valueFunc), minValue(minVal), maxValue(maxVal), numBins(bins), maxBarWidth(maxWidth) {
        if (minVal >= maxVal) {
            errorsDetection(Error::INVALID_RANGE);
            throw std::invalid_argument("Invalid range: minValue must be less than maxValue");
        }
        if (bins <= 0) {
            errorsDetection(Error::INVALID_ARGUMENT);
            throw std::invalid_argument("Number of bins must be positive");
        }
        
        binSize = (maxValue - minValue + numBins - 1) / numBins;
        if (binSize == 0) binSize = 1;
    }

    std::string getRangeLabel(int binIndex) const {
        int start = minValue + binIndex * binSize;
        int end = start + binSize - 1;
        if (binIndex == numBins - 1) {
            end = maxValue; 
        }
        return std::to_string(start) + "-" + std::to_string(end);
    }

    HashDictionary<std::string, int> buildHistogram(const std::vector<T>& data) {
        HashDictionary<std::string, int> histogram(20);
        
        for (const auto& item : data) {
            int value = getValue(item); //size_t лучше а не все ок
            int binIndex = (value - minValue) / binSize;

            if (binIndex < 0) {
                binIndex = 0;
            }
            if (binIndex >= numBins) {
                binIndex = numBins - 1;
            }
            
            std::string rangeKey = getRangeLabel(binIndex);
            
            if (histogram.contains(rangeKey)){
                histogram.at(rangeKey) = histogram.at(rangeKey) + 1;
            } else {
                histogram.insert(rangeKey, 1);
            }
        }
        return histogram;
    }

    void printHorizontalHistogram(const HashDictionary<std::string, int>& histogram) {
        auto keys = histogram.keys();
        
        if (keys.empty()) {
            std::cout << "Histogram is empty" << std::endl;
            return;
        }
        std::sort(keys.begin(), keys.end());
        int maxCount = 0;
        for (const auto& key : keys) {
            auto count = histogram.at(key);
            if (count > maxCount) {
                maxCount = count;
            }
        }
        
        for (const auto& key : keys) {
            auto count = histogram.at(key);
            
            int barWidth = maxCount > 0 ? (count * maxBarWidth) / maxCount : 0;
            
            std::string bar(barWidth, '#');
            
            std::cout << key << ": " << bar;
            
            if (barWidth < maxBarWidth) {
                std::cout << std::string(maxBarWidth - barWidth, ' ');
            }
            
            std::cout << "  (" << count << ")  " << std::endl;
        }
    }

    void printVerticalHistogram(const HashDictionary<std::string, int>& histogram) {
        auto keys = histogram.keys();
        
        if (keys.empty()) {
            std::cout << "Histogram is empty" << std::endl;
            return;
        }
        std::sort(keys.begin(), keys.end());
        int maxCount = 0;
        for (const auto& key : keys) {
            auto count = histogram.at(key);
            if (count > maxCount) {
                maxCount = count;
            }
        }
        
        int graphHeight = std::min(maxCount, 50);

        for (int level = graphHeight; level > 0; level--) {
            for (const auto& key : keys) {
                auto count = histogram.at(key);
                int scaledHeight = (count * graphHeight) / maxCount;
                
                if (scaledHeight >= level) {
                    std::cout << "  ***  ";
                } else {
                    std::cout << "     ";
                }
            }
            std::cout << std::endl;
        }
        
        for (size_t i = 0; i < keys.size(); i++) {
            std::cout << "-----";
        }
        std::cout << std::endl;
        
        for (const auto& key : keys) {
            std::cout << " " << key << " "; 
        }
        std::cout << std::endl;
        
        for (const auto& key : keys) {
            int count = histogram.at(key);
            std::cout << "  (" << count << ")  ";
        }
        std::cout << std::endl;
    }
};

template<typename T>
class TreeHistogram {
private:
    std::function<int(T)> getValue;
    int minValue;
    int maxValue;
    int binSize;
    int numBins;
    int maxBarWidth;

public:
    TreeHistogram(std::function<int(T)> valueFunc, int minVal, int maxVal, int bins, int maxWidth = 50)
        : getValue(valueFunc), minValue(minVal), maxValue(maxVal), numBins(bins), maxBarWidth(maxWidth) {
        if (minVal >= maxVal) {
            errorsDetection(Error::INVALID_RANGE);
            throw std::invalid_argument("Invalid range: minValue must be less than maxValue");
        }
        if (bins <= 0) {
            errorsDetection(Error::INVALID_ARGUMENT);
            throw std::invalid_argument("Number of bins must be positive");
        }
        
        binSize = (maxValue - minValue + numBins - 1) / numBins;
        if (binSize == 0) binSize = 1;
    }

    std::string getRangeLabel(int binIndex) const {
        int start = minValue + binIndex * binSize;
        int end = start + binSize - 1;
        if (binIndex == numBins - 1) {
            end = maxValue; 
        }
        return std::to_string(start) + "-" + std::to_string(end);
    }

    TreeDictionary<std::string, int> buildHistogram(const std::vector<T>& data) {
        TreeDictionary<std::string, int> histogram;
        
        for (const auto& item : data) {
            int value = getValue(item);
            int binIndex = (value - minValue) / binSize;

            if (binIndex < 0) {
                binIndex = 0;
            }
            if (binIndex >= numBins) {
                binIndex = numBins - 1;
            }
            
            std::string rangeKey = getRangeLabel(binIndex);
            
            if (histogram.contains(rangeKey)){
                histogram.at(rangeKey) = histogram.at(rangeKey) + 1;
            } else {
                histogram.insert(rangeKey, 1);
            }
        }
        return histogram;
    }

    void printHorizontalHistogram(const TreeDictionary<std::string, int>& histogram) {
        auto keys = histogram.keys();
        
        if (keys.empty()) {
            std::cout << "Histogram is empty" << std::endl;
            return;
        }
        
        int maxCount = 0;
        for (const auto& key : keys) {
            auto count = histogram.at(key);
            if (count > maxCount) {
                maxCount = count;
            }
        }
        
        for (const auto& key : keys) {
            auto count = histogram.at(key);
            
            int barWidth = maxCount > 0 ? (count * maxBarWidth) / maxCount : 0;
            
            std::string bar(barWidth, '#');
            
            std::cout << key << ": " << bar;
            
            if (barWidth < maxBarWidth) {
                std::cout << std::string(maxBarWidth - barWidth, ' ');
            }
            
            std::cout << " (" << count << ")" << std::endl;
        }
    }

    void printVerticalHistogram(const TreeDictionary<std::string, int>& histogram) {
        auto keys = histogram.keys();
        
        if (keys.empty()) {
            std::cout << "Histogram is empty" << std::endl;
            return;
        }
        
        int maxCount = 0;
        for (const auto& key : keys) {
            auto count = histogram.at(key);
            if (count > maxCount) {
                maxCount = count;
            }
        }
        
        int graphHeight = std::min(maxCount, 20);
        

        for (int level = graphHeight; level > 0; level--) {
            for (const auto& key : keys) {
                auto count = histogram.at(key);
                int scaledHeight = (count * graphHeight) / maxCount;
                
                if (scaledHeight >= level) {
                    std::cout << "  ***  ";
                } else {
                    std::cout << "     ";
                }
            }
            std::cout << std::endl;
        }
        
        for (size_t i = 0; i < keys.size(); i++) {
            std::cout << "-----";
        }
        std::cout << std::endl;
        
        for (const auto& key : keys) {
            std::cout << " " << key.substr(0, std::min(3, (int)key.length())) << " "; 
        }
        std::cout << std::endl;
        
        for (const auto& key : keys) {
            int count = histogram.at(key);
            std::cout << "(" << count << ") ";
        }
        std::cout << std::endl;
    }
};

#endif 