#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <limits>
#include <algorithm>
#include "histogram.hpp"
#include "cache.hpp"
#include "IDictionary.hpp"

using namespace std;

// Forward declarations
void workWithHashHistogram();
void workWithTreeHistogram();
void workWithHashCache();
void workWithTreeCache();

void mainMenu(){
    cout << "\n=== MAIN MENU ===\n";
    cout << "1. Histogram Operations\n";
    cout << "2. Caching Operations\n";
    cout << "3. Tree Operations\n";
    cout << "4. Hash Table Operations\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

void histogramMenu(){
    cout << "\n=== HISTOGRAM MENU ===\n";
    cout << "1. Use HashHistogram\n";
    cout << "2. Use TreeHistogram\n";
    cout << "0. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

void cacheMenu(){
    cout << "\n=== CACHE MENU ===\n";
    cout << "1. Use HashLRUCache\n";
    cout << "2. Use TreeLRUCache\n";
    cout << "0. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

void treeOperationsMenu(){
    cout << "\n=== TREE OPERATIONS MENU ===\n";
    cout << "1. Insert key-value pair\n";
    cout << "2. Find value by key\n";
    cout << "3. Remove key\n";
    cout << "4. Check if tree is empty\n";
    cout << "5. Print tree structure\n";
    cout << "6. Clear tree\n";
    cout << "7. Get tree size\n";
    cout << "8. Get all keys\n";
    cout << "0. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

void hashOperationsMenu(){
    cout << "\n=== HASH TABLE OPERATIONS MENU ===\n";
    cout << "1. Insert key-value pair\n";
    cout << "2. Get value by key\n";
    cout << "3. Remove key\n";
    cout << "4. Check if key exists\n";
    cout << "5. Get all keys\n";
    cout << "6. Get all values\n";
    cout << "7. Print hash table statistics\n";
    cout << "8. Clear hash table\n";
    cout << "9. Check if empty\n";
    cout << "10. Get size\n";
    cout << "0. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

void histogramOperationsMenu(bool isHashHistogram){
    cout << "\n=== HISTOGRAM OPERATIONS ===\n";
    if (isHashHistogram) {
        cout << "Using: HashHistogram\n";
    } else {
        cout << "Using: TreeHistogram\n";
    }
    cout << "1. Build histogram from data\n";
    cout << "2. Print horizontal histogram\n";
    cout << "3. Print vertical histogram\n";
    cout << "0. Return to Histogram Menu\n";
    cout << "Enter your choice: ";
}

void cacheOperationsMenu(bool isHashCache){
    cout << "\n=== CACHE OPERATIONS ===\n";
    if (isHashCache) {
        cout << "Using: HashLRUCache\n";
    } else {
        cout << "Using: TreeLRUCache\n";
    }
    cout << "1. Put item in cache\n";
    cout << "2. Get item from cache\n";
    cout << "3. Remove item from cache\n";
    cout << "4. Clear cache\n";
    cout << "5. Print cache statistics\n";
    cout << "6. Get cache size\n";
    cout << "7. Get cache capacity\n";
    cout << "0. Return to Cache Menu\n";
    cout << "Enter your choice: ";
}

void workWithHistogram(){
    int choice;
    do{
        histogramMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch(choice){
            case 1:
                workWithHashHistogram();
                break;
            case 2:
                workWithTreeHistogram();
                break;
            case 0:
                cout << "Returning to Main Menu\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while(choice != 0);
}

void workWithCache(){
    int choice;
    do{
        cacheMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch(choice){
            case 1:
                workWithHashCache();
                break;
            case 2:
                workWithTreeCache();
                break;
            case 0:
                cout << "Returning to Main Menu\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while(choice != 0);
}

void workWithTree(){
    TreeDictionary<int, string> treeDict;
    int choice;
    
    do{
        treeOperationsMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        try{
            switch(choice){
                case 1:{
                    cout << "Enter key (integer): ";
                    int key;
                    if (!(cin >> key)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid key. Must be an integer.\n";
                        break;
                    }
                    cout << "Enter value (string): ";
                    string value;
                    cin.ignore();
                    getline(cin, value);
                    treeDict.insert(key, value);
                    cout << "Inserted successfully\n";
                    break;
                }
                case 2:{
                    cout << "Enter key to find: ";
                    int key;
                    if (!(cin >> key)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid key. Must be an integer.\n";
                        break;
                    }
                    if(treeDict.contains(key)){
                        string value = treeDict.at(key);
                        cout << "Value: " << value << endl;
                    } else{
                        cout << "Key not found\n";
                    }
                    break;
                }
                case 3:{
                    cout << "Enter key to remove: ";
                    int key;
                    if (!(cin >> key)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid key. Must be an integer.\n";
                        break;
                    }
                    if(treeDict.contains(key)){
                        string value = treeDict.at(key); 
                        treeDict.erase(key, value); 
                        cout << "Key removed successfully\n";
                    } else {
                        cout << "Key not found\n";
                    }
                    break;
                }
                case 4:{
                    if(treeDict.empty()){
                        cout << "Tree is empty\n";
                    } else{
                        cout << "Tree is not empty (size: " << treeDict.size() << ")\n";
                    }
                    break;
                }
                case 5:{
                    cout << "Tree structure:\n";
                    treeDict.print();
                    break;
                }
                case 6:{
                    treeDict.clear();
                    cout << "Tree cleared\n";
                    break;
                }
                case 7:{
                    cout << "Tree size: " << treeDict.size() << endl;
                    break;
                }
                case 8:{
                    vector<int> keys = treeDict.keys();
                    cout << "All keys: ";
                    for (int k : keys) {
                        cout << k << " ";
                    }
                    cout << endl;
                    break;
                }
                case 0:{
                    cout << "Returning to Main Menu\n";
                    break;
                }
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } catch(const exception& e){
            cout << "Error: " << e.what() << endl;
        }
    } while(choice != 0);
}

void workWithHashTable(){
    cout << "Enter initial capacity for hash table: ";
    size_t capacity;
    if (!(cin >> capacity) || capacity <= 0) {
        cout << "Invalid capacity. Using default capacity 10.\n";
        capacity = 10;
    }
    
    HashDictionary<int, string> hashDict(capacity);
    int choice;
    
    do{
        hashOperationsMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        try{
            switch(choice){
                case 1:{
                    cout << "Enter key (integer): ";
                    int key;
                    if (!(cin >> key)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid key. Must be an integer.\n";
                        break;
                    }
                    cout << "Enter value (string): ";
                    string value;
                    cin.ignore();
                    getline(cin, value);
                    hashDict.insert(key, value);
                    cout << "Inserted successfully\n";
                    break;
                }
                case 2:{
                    cout << "Enter key to get: ";
                    int key;
                    if (!(cin >> key)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid key. Must be an integer.\n";
                        break;
                    }
                    if(hashDict.contains(key)){
                        string value = hashDict.at(key);
                        cout << "Value: " << value << endl;
                    } else{
                        cout << "Key not found\n";
                    }
                    break;
                }
                case 3:{
                    cout << "Enter key to remove: ";
                    int key;
                    if (!(cin >> key)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid key. Must be an integer.\n";
                        break;
                    }
                    if (hashDict.contains(key)) {
                        hashDict.erase(key);
                        cout << "Key removed successfully\n";
                    } else {
                        cout << "Key not found\n";
                    }
                    break;
                }
                case 4:{
                    cout << "Enter key to check: ";
                    int key;
                    if (!(cin >> key)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid key. Must be an integer.\n";
                        break;
                    }
                    if(hashDict.contains(key)){
                        cout << "Key exists\n";
                    } else{
                        cout << "Key does not exist\n";
                    }
                    break;
                }
                case 5:{
                    vector<int> keys = hashDict.keys();
                    cout << "All keys (" << keys.size() << "): ";
                    for(int k : keys){
                        cout << k << " ";
                    }
                    cout << endl;
                    break;
                }
                case 6:{
                    vector<string> values = hashDict.values();
                    cout << "All values (" << values.size() << "): ";
                    for(const string& v : values){
                        cout << v << " ";
                    }
                    cout << endl;
                    break;
                }
                case 7:{
                    hashDict.printStats();
                    break;
                }
                case 8:{
                    hashDict.clear();
                    cout << "Hash table cleared\n";
                    break;
                }
                case 9:{
                    if(hashDict.empty()){
                        cout << "Hash table is empty\n";
                    } else{
                        cout << "Hash table is not empty (size: " << hashDict.size() << ")\n";
                    }
                    break;
                }
                case 10:{
                    cout << "Size: " << hashDict.size() << endl;
                    break;
                }
                case 0:{
                    cout << "Returning to Main Menu\n";
                    break;
                }
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } catch(const exception& e){
            cout << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while(choice != 0);
}

void workWithHashHistogram(){
    int bins, maxWidth, width;
    
    cout << "\n=== CONFIGURE HASH HISTOGRAM ===\n";
    
    cout << "Enter number of bins: ";
    cin >> bins;
    cout << "Enter maximum bar width (default 50): ";
    cin << width;
    maxWidth = min(50, width)
    
    vector<int> data;
    HashDictionary<string, int> histogram(20);
    bool histogramBuilt = false;
    int minVal = 0, maxVal = 0;
    
    int choice;
    do{
        histogramOperationsMenu(true);
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch(choice){
            case 1:{
                cout << "Enter numbers (one per line, enter -1 to finish):\n";
                data.clear();
                int num;
                while(true){
                    if (!(cin >> num)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a number or -1 to finish: ";
                        continue;
                    }
                    if(num == -1) break;
                    data.push_back(num);
                }
                
                if (data.empty()) {
                    cout << "No data entered. Histogram not built.\n";
                    break;
                }
                
                minVal = data[0];
                maxVal = data[0];
                for (int value : data) {
                    if (value < minVal) minVal = value;
                    if (value > maxVal) maxVal = value;
                }
                
                if (minVal == maxVal) {
                    maxVal = minVal + 1;
                }
                
                cout << "\nData analysis:\n";
                cout << "  Min value: " << minVal << endl;
                cout << "  Max value: " << maxVal << endl;
                cout << "  Number of bins: " << bins << endl;
                cout << "  Total data points: " << data.size() << endl;
                
                HashHistogram<int> hist(
                    [](int x){ return x; },
                    minVal, maxVal, bins, maxWidth
                );
                
                histogram = hist.buildHistogram(data);
                histogramBuilt = true;
                cout << "\nHistogram built successfully!\n";
                break;
            }
            case 2:{
                if (!histogramBuilt || histogram.empty()) {
                    cout << "Histogram is empty or not built yet. Build it first (option 1).\n";
                } else {
                    cout << "\n=== HORIZONTAL HISTOGRAM ===\n";
                    
                    if (!data.empty()) {
                        HashHistogram<int> hist(
                            [](int x){ return x; },
                            minVal, maxVal, bins, maxWidth
                        );
                        hist.printHorizontalHistogram(histogram);
                    }
                }
                break;
            }
            case 3:{
                if (!histogramBuilt || histogram.empty()) {
                    cout << "Histogram is empty or not built yet. Build it first (option 1).\n";
                } else {
                    cout << "\n=== VERTICAL HISTOGRAM ===\n";
                    
                    if (!data.empty()) {
                        HashHistogram<int> hist(
                            [](int x){ return x; },
                            minVal, maxVal, bins, maxWidth
                        );
                        hist.printVerticalHistogram(histogram);
                    }
                }
                break;
            }
            case 0:{
                cout << "Returning to Histogram Menu\n";
                break;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while(choice != 0);
}

void workWithTreeHistogram(){
    int bins, maxWidth;
    
    cout << "\n=== CONFIGURE TREE HISTOGRAM ===\n";
    
    cout << "Enter number of bins: ";
    if (!(cin >> bins) || bins <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Using default 10 bins.\n";
        bins = 10;
    }
    
    cout << "Enter maximum bar width (default 50): ";
    if (!(cin >> maxWidth) || maxWidth <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        maxWidth = 50;
    }
    
    vector<int> data;
    TreeDictionary<string, int> histogram;
    bool histogramBuilt = false;
    int minVal = 0, maxVal = 0;
    
    int choice;
    do{
        histogramOperationsMenu(false);
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch(choice){
            case 1:{
                cout << "Enter numbers (one per line, enter -1 to finish):\n";
                data.clear();
                int num;
                while(true){
                    if (!(cin >> num)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a number or -1 to finish: ";
                        continue;
                    }
                    if(num == -1) break;
                    data.push_back(num);
                }
                
                if (data.empty()) {
                    cout << "No data entered. Histogram not built.\n";
                    break;
                }
                
                // Находим минимальное и максимальное значение в данных
                minVal = data[0];
                maxVal = data[0];
                for (int value : data) {
                    if (value < minVal) minVal = value;
                    if (value > maxVal) maxVal = value;
                }
                
                // Если все значения одинаковые, расширяем диапазон
                if (minVal == maxVal) {
                    maxVal = minVal + 1;
                }
                
                cout << "\nData analysis:\n";
                cout << "  Min value: " << minVal << endl;
                cout << "  Max value: " << maxVal << endl;
                cout << "  Number of bins: " << bins << endl;
                cout << "  Total data points: " << data.size() << endl;
                
                // Создаем гистограмму с вычисленными параметрами
                TreeHistogram<int> hist(
                    [](int x){ return x; },
                    minVal, maxVal, bins, maxWidth
                );
                
                try {
                    histogram = hist.buildHistogram(data);
                    histogramBuilt = true;
                    cout << "\nHistogram built successfully!\n";
                } catch (const exception& e) {
                    cout << "Error building histogram: " << e.what() << endl;
                }
                break;
            }
            case 2:{
                if (!histogramBuilt || histogram.empty()) {
                    cout << "Histogram is empty or not built yet. Build it first (option 1).\n";
                } else {
                    cout << "\n=== HORIZONTAL HISTOGRAM ===\n";
                    
                    // Получаем гистограмму для отображения
                    if (!data.empty()) {
                        TreeHistogram<int> hist(
                            [](int x){ return x; },
                            minVal, maxVal, bins, maxWidth
                        );
                        hist.printHorizontalHistogram(histogram);
                    }
                }
                break;
            }
            case 3:{
                if (!histogramBuilt || histogram.empty()) {
                    cout << "Histogram is empty or not built yet. Build it first (option 1).\n";
                } else {
                    cout << "\n=== VERTICAL HISTOGRAM ===\n";
                    
                    // Получаем гистограмму для отображения
                    if (!data.empty()) {
                        TreeHistogram<int> hist(
                            [](int x){ return x; },
                            minVal, maxVal, bins, maxWidth
                        );
                        hist.printVerticalHistogram(histogram);
                    }
                }
                break;
            }
            case 0:{
                cout << "Returning to Histogram Menu\n";
                break;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while(choice != 0);
}

void workWithHashCache(){
    cout << "Enter cache capacity: ";
    size_t capacity;
    if (!(cin >> capacity) || capacity <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid capacity. Using default capacity 100.\n";
        capacity = 100;
    }
    
    HashLRUCache<int, string> cache(capacity);
    int choice;
    
    do{
        cacheOperationsMenu(true);
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch(choice){
            case 1:{
                cout << "Enter key (integer): ";
                int key;
                if (!(cin >> key)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid key. Must be an integer.\n";
                    break;
                }
                cout << "Enter value: ";
                string value;
                cin.ignore();
                getline(cin, value);
                cache.put(key, value);
                cout << "Item added to cache\n";
                break;
            }
            case 2:{
                cout << "Enter key: ";
                int key;
                if (!(cin >> key)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid key. Must be an integer.\n";
                    break;
                }
                string value;
                if(cache.get(key, value)){
                    cout << "Value: " << value << endl;
                } else{
                    cout << "Key not found in cache\n";
                }
                break;
            }
            case 3:{
                cout << "Enter key to remove: ";
                int key;
                if (!(cin >> key)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid key. Must be an integer.\n";
                    break;
                }
                if (cache.size() > 0) {
                    string dummy;
                    if (cache.get(key, dummy)) {
                        cache.remove(key);
                        cout << "Item removed from cache\n";
                    } else {
                        cout << "Key not found in cache\n";
                    }
                } else {
                    cout << "Cache is empty\n";
                }
                break;
            }
            case 4:{
                cache.clear();
                cout << "Cache cleared\n";
                break;
            }
            case 5:{
                cache.printStats();
                break;
            }
            case 6:{
                cout << "Cache size: " << cache.size() << endl;
                break;
            }
            case 7:{
                cout << "Cache capacity: " << cache.capacity() << endl;
                break;
            }
            case 0:{
                cout << "Returning to Cache Menu\n";
                break;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while(choice != 0);
}

void workWithTreeCache(){
    cout << "Enter cache capacity: ";
    size_t capacity;
    if (!(cin >> capacity) || capacity <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid capacity. Using default capacity 100.\n";
        capacity = 100;
    }
    
    TreeLRUCache<int, string> cache(capacity);
    int choice;
    
    do{
        cacheOperationsMenu(false);
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch(choice){
            case 1:{
                cout << "Enter key (integer): ";
                int key;
                if (!(cin >> key)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid key. Must be an integer.\n";
                    break;
                }
                cout << "Enter value: ";
                string value;
                cin.ignore();
                getline(cin, value);
                cache.put(key, value);
                cout << "Item added to cache\n";
                break;
            }
            case 2:{
                cout << "Enter key: ";
                int key;
                if (!(cin >> key)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid key. Must be an integer.\n";
                    break;
                }
                string value;
                if(cache.get(key, value)){
                    cout << "Value: " << value << endl;
                } else{
                    cout << "Key not found in cache\n";
                }
                break;
            }
            case 3:{
                cout << "Enter key to remove: ";
                int key;
                if (!(cin >> key)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid key. Must be an integer.\n";
                    break;
                }
                if (cache.size() > 0) {
                    string dummy;
                    if (cache.get(key, dummy)) {
                        cache.remove(key);
                        cout << "Item removed from cache\n";
                    } else {
                        cout << "Key not found in cache\n";
                    }
                } else {
                    cout << "Cache is empty\n";
                }
                break;
            }
            case 4:{
                cache.clear();
                cout << "Cache cleared\n";
                break;
            }
            case 5:{
                cache.printStats();
                break;
            }
            case 6:{
                cout << "Cache size: " << cache.size() << endl;
                break;
            }
            case 7:{
                cout << "Cache capacity: " << cache.capacity() << endl;
                break;
            }
            case 0:{
                cout << "Returning to Cache Menu\n";
                break;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while(choice != 0);
}

void runInterface(){
    int choice;
    do{
        mainMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch(choice){
            case 1:
                workWithHistogram();
                break;
            case 2:
                workWithCache();
                break;
            case 3:
                workWithTree();
                break;
            case 4:
                workWithHashTable();
                break;
            case 0:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while(choice != 0);
}

#endif