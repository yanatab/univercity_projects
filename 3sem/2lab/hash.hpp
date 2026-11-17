#ifndef HASH_HPP
#define HASH_HPP

#include <iostream>
#include <list>
#include <vector>
#include <functional>
#include <string>
#include "errors.h"

enum class State {
    EMPTY,
    OCCUPIED,
    DELETED
};

template <typename K, typename V>
class HashNode {
public:
    K key;
    V value;
    State state;

    HashNode() : state(State::EMPTY) {}
    HashNode(const K& k, const V& v) : key(k), value(v), state(State::OCCUPIED) {}
};

template<typename K, typename V>
class HashTable {
private: //приват на черточку заканчивается
    std::vector<HashNode<K, V>> table;
    size_t capacity;
    size_t size_;
    float loadFactor;

    size_t hashFunction(const K& key, size_t cap) const{
        std::hash<K> hasher;
        return hasher(key) % cap;
    }

    size_t shiftFunction(size_t index, size_t att, size_t capacity) const{
        return (index + att) % capacity;
    }

    void rehash() {
        size_t newCapacity = capacity * 2;
        if (capacity == 0) {
            newCapacity = 10; 
        }
        
        std::vector<HashNode<K, V>> oldTable = std::move(table);
        size_t oldCapacity = capacity;
        
        table.resize(newCapacity);
        capacity = newCapacity;
        size_ = 0;

        for (size_t i = 0; i < newCapacity; i++) {
            table[i].state = State::EMPTY;
        }
        
        for (size_t i = 0; i < oldCapacity; i++) {
            if (oldTable[i].state == State::OCCUPIED) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
    }

    size_t findIndex(const K& key, bool forInsert) const {
        if (capacity == 0) {
            return 0;
        }

        size_t startIndex = hashFunction(key, capacity);
        size_t index = startIndex;
        size_t att = 0;
        int firstDeleted = -1;
        bool looped = false;

        do {
            if (table[index].state == State::EMPTY) {
                if (forInsert && firstDeleted != -1) {
                    return firstDeleted;
                }
                return index;
            }

            if (table[index].state == State::DELETED) {
                if (firstDeleted == -1) {
                    firstDeleted = index;
                }
            }
            else if (table[index].state == State::OCCUPIED && table[index].key == key) {
                return index;
            }
            att++;
            index = shiftFunction(startIndex, att, capacity);

            if (att > capacity) {
                break;
            }

        } while (index != startIndex);

        if (forInsert && firstDeleted != -1) {
            return firstDeleted;
        }
        return index;
    }

public:
    HashTable(float lf = 0.75) : capacity(10), size_(0), loadFactor(lf){
        table.resize(capacity);
    }
    
    ~HashTable() = default;

    void insert(const K& key, const V& value) {
        if (capacity == 0 || (float)(size_) / capacity >= loadFactor) {
            rehash();
        }
        
        size_t index = findIndex(key, true);
        
        if (table[index].state != State::OCCUPIED) {
            table[index] = HashNode<K, V>(key, value);
            table[index].state = State::OCCUPIED;
            size_++;
        }
    }
    
    bool get(const K& key, V& value) {
        if (capacity == 0) {
            return false;
        }
        size_t index = findIndex(key, false);
        if (table[index].state == State::OCCUPIED && table[index].key == key && table[index].value == value) {
            return true;
        }
        return false;
    }

    V* getRef(const K& key){
        size_t index = findIndex(key, false);
        if (capacity == 0) {
            return nullptr;
        }
        
        if (table[index].state == State::OCCUPIED && table[index].key == key) {
            return &(table[index].value);
        }
        return nullptr;
    }

    const V* getRef(const K& key) const {
        size_t index = findIndex(key, false);
        if (capacity == 0) {
            return nullptr;
        }
        
        if (table[index].state == State::OCCUPIED && table[index].key == key) {
            return &(table[index].value);
        }
        return nullptr;
    }


    void deleted(const K& key) {
        size_t index = findIndex(key, false);
        if (capacity == 0) {
            return;
        }
        
        if (table[index].state == State::OCCUPIED && table[index].key == key) {
            table[index].state = State::DELETED;
            size_--;
        }
    }

    size_t getSize() const {
        return size_;
    }

    size_t getCapacity() const {
        return capacity;
    }

    float getCurrentLoadFactor() const {
        if (capacity == 0) {
            return 0;
        }
        return (float)size_ / capacity;
    }

    
    std::vector<K> keys() const {
        std::vector<K> keyList;
        for (size_t i = 0; i < capacity; i++) {
            if (table[i].state == State::OCCUPIED) {
                keyList.push_back(table[i].key);
            }
        }
        return keyList;
    }

    std::vector<V> values() const {
        std::vector<V> valueList;
        for (size_t i = 0; i < capacity; i++) {
            if (table[i].state == State::OCCUPIED) {
                valueList.push_back(table[i].value);
            }
        }
        return valueList;
    }
    
    void clear() {
        for (size_t i = 0; i < capacity; i++) {
            table[i].state = State::EMPTY;
        }
        size_ = 0;
    }
    
    bool contains(const K& key) const {
        size_t index = findIndex(key, false);
        return table[index].state == State::OCCUPIED && table[index].key == key;
    }
};

#endif 