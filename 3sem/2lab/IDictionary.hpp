#ifndef IDICTIONARY_HPP
#define IDICTIONARY_HPP

#include "tree.hpp"
#include "hash.hpp"
#include "errors.h"
#include <vector>
#include <stdexcept>

template<typename K, typename V>
class TreeDictionary {
private:
    BinaryTree<K, V> tree;

public:
    TreeDictionary() = default;
    ~TreeDictionary() = default;
    
    void insert(const K& key, const V& value) {
        tree.insert(key, value);
    }
    
    V& at(const K& key) {
        V* value = tree.find(key);
        if (value == nullptr) {
            errorsDetection(Error::KEY_NOT_FOUND);
            throw std::out_of_range("Key not found");
        }
        return *value;
    }

    const V& at(const K& key) const {
        const V* value = tree.find(key);
        if (value == nullptr) {
            errorsDetection(Error::KEY_NOT_FOUND);
            throw std::out_of_range("Key not found");
        }
        return *value;
    }

    V& operator[](const K& key) {
        V* value = tree.find(key);
        if (value == nullptr) {
            tree.insert(key, V());
            value = tree.find(key);
        }
        return *value;
    }

    bool contains(const K& key) const {
        return tree.findKey(key);
    }
    
    void erase(const K& key, const V& value) {
        tree.remove(key, value);
    }
    
    void clear() {
        tree.clear();
    }
    
    bool empty() const {
        return tree.empty();
    }
    
    size_t size() const {
        return tree.size();
    }
    
    std::vector<K> keys() const {
        return tree.keys();
    }
    
    void print() const {
        tree.printTree();
    }
};

template<typename K, typename V>
class HashDictionary {
private:
    HashTable<K, V> hashTable;

public:
    HashDictionary() : hashTable(10) {}
    HashDictionary(size_t initialCapacity) : hashTable(initialCapacity) {}
    
    ~HashDictionary() = default;
    
    void insert(const K& key, const V& value) {
        hashTable.insert(key, value);
    }
    
    V& at(const K& key) {
        V* value = hashTable.getRef(key);
        if (value == nullptr) {
            errorsDetection(Error::KEY_NOT_FOUND);
            throw std::out_of_range("Key not found");
        }
        return *value;
    }

    const V& at(const K& key) const {
        const V* value = hashTable.getRef(key);
        if (value == nullptr) {
            errorsDetection(Error::KEY_NOT_FOUND);
            throw std::out_of_range("Key not found");
        }
        return *value;
    }

    
    V& operator[](const K& key) {
        V* value = hashTable.getRef(key);
        if (value == nullptr) {
            hashTable.insert(key, V());
            value = hashTable.getRef(key);
        }
        return *value;
    }
    
    bool contains(const K& key) const {
        return hashTable.contains(key);
    }
    
    void erase(const K& key) {
        hashTable.deleted(key);
    }
    
    void clear() {
        hashTable.clear();
    }
    
    bool empty() const {
        return hashTable.getSize() == 0;
    }
    
    size_t size() const {
        return hashTable.getSize();
    }
    
    std::vector<K> keys() const {
        return hashTable.keys();
    }
    
    std::vector<V> values() const {
        return hashTable.values();
    }
    
    void printStats() const {
        std::cout << "Size: " << hashTable.getSize() << std::endl;
        std::cout << "Capacity: " << hashTable.getCapacity() << std::endl;
        std::cout << "Load Factor: " << hashTable.getCurrentLoadFactor() << std::endl;
    }
};

#endif