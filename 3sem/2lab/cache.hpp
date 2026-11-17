#ifndef CACHE_HPP
#define CACHE_HPP

#include "IDictionary.hpp"
#include "errors.h"
#include <iostream>
#include <chrono>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <thread>

template<typename K, typename V>
class ICache {
public:
    virtual ~ICache() = default;
    virtual bool get(const K& key, V& value) = 0;
    virtual void put(const K& key, const V& value) = 0;
    virtual void remove(const K& key) = 0;
    virtual void clear() = 0;
    virtual size_t size() const = 0;
    virtual size_t capacity() const = 0;
    virtual void printStats() const = 0;
};

template<typename K, typename V>
class HashLRUCache : public ICache<K, V> {
private:
    HashDictionary<K, V> cache;
    std::list<K> accessOrder;
    size_t Capacity;
    
    void updateAccess(const K& key) {
        auto it = std::find(accessOrder.begin(), accessOrder.end(), key);
        if (it != accessOrder.end()) {
            accessOrder.erase(it);
        }
        accessOrder.push_front(key);
    }
    
    void evictOldest() {
        if (accessOrder.empty()) return;
        
        K oldestKey = accessOrder.back();
        accessOrder.pop_back();
        cache.erase(oldestKey);
    }

public:
    HashLRUCache(size_t capacity = 100) : cache(capacity), Capacity(capacity) {
        cache.clear();
        accessOrder.clear();
    }
    
    bool get(const K& key, V& value) override {
        if (cache.contains(key)) {
            value = cache.at(key);
            updateAccess(key);
            return true;
        }
        return false;
    }
    
    void put(const K& key, const V& value) override {
        if (cache.size() >= Capacity) {
            evictOldest();
        }
        
        cache.insert(key, value);
        updateAccess(key);
    }
    
    void remove(const K& key) override {
        cache.erase(key);
        auto it = std::find(accessOrder.begin(), accessOrder.end(), key);
        if (it != accessOrder.end()) {
            accessOrder.erase(it);
        }
    }
    
    void clear() override {
        cache.clear();
        accessOrder.clear();
    }
    
    size_t size() const override {
        return cache.size();
    }
    
    size_t capacity() const override {
        return Capacity;
    }
    
    void printStats() const override {
        std::cout << "Hash LRU Cache Stats:" << std::endl;
        std::cout << "  Capacity: " << Capacity << std::endl;
        std::cout << "  Current size: " << size() << std::endl;
        std::cout << "  Load factor: " << (double)size() / Capacity * 100 << "%" << std::endl;
        
        if (!accessOrder.empty()) {
            std::cout << "  Most recent: " << accessOrder.front() << std::endl;
            std::cout << "  Least recent: " << accessOrder.back() << std::endl;
            std::cout << "  Access order: ";
                for (const K& key : accessOrder) {
                    std::cout << key << " ";
                } 
        } else {
        std::cout << "  Cache is empty" << std::endl;
        }
    }
};

template<typename K, typename V>
class TreeLRUCache : public ICache<K, V> {
private:
    TreeDictionary<K, V> cache;
    std::list<K> accessOrder;
    size_t Capacity;
    
    void updateAccess(const K& key) {
        auto it = std::find(accessOrder.begin(), accessOrder.end(), key);
        if (it != accessOrder.end()) {
            accessOrder.erase(it);
        }
        accessOrder.push_front(key);
    }
    
    void evictOldest() {
        if (accessOrder.empty()) return;
        
        K oldestKey = accessOrder.back();
        accessOrder.pop_back();
        if (cache.contains(oldestKey)) {
            V value = cache.at(oldestKey);
            cache.erase(oldestKey, value);
        }
    }

public:
    TreeLRUCache(size_t capacity = 100) : Capacity(capacity) {
        cache.clear();
        accessOrder.clear();
    }
    
    bool get(const K& key, V& value) override {
        if (cache.contains(key)) {
            value = cache.at(key);
            updateAccess(key);
            return true;
        }
        return false;
    }
    
    void put(const K& key, const V& value) override {
        if (cache.size() >= Capacity) {
            evictOldest();
        }
        
        cache.insert(key, value);
        updateAccess(key);
    }
    
    void remove(const K& key) override {
        if (cache.contains(key)) {
            V value = cache.at(key);  
            cache.erase(key, value); 
        } else return;
        auto it = std::find(accessOrder.begin(), accessOrder.end(), key);
        if (it != accessOrder.end()) {
            accessOrder.erase(it);
        }
    }
    
    void clear() override {
        cache.clear();
        accessOrder.clear();
    }
    
    size_t size() const override {
        return cache.size();
    }
    
    size_t capacity() const override {
        return Capacity;
    }
    
    void printStats() const override {
        std::cout << "Tree LRU Cache Stats:" << std::endl;
        std::cout << "  Capacity: " << Capacity << std::endl;
        std::cout << "  Current size: " << size() << std::endl;
        std::cout << "  Load factor: " << (double)size() / Capacity * 100 << "%" << std::endl;
        
        if (!accessOrder.empty()) {
            std::cout << "  Most recent: " << accessOrder.front() << std::endl;
            std::cout << "  Least recent: " << accessOrder.back() << std::endl;
            std::cout << "  Access order: ";
            for (const K& key : accessOrder) {
                std::cout << key << " ";
            } 
        } else {
        std::cout << "  Cache is empty" << std::endl;
        }
    }
};

#endif 