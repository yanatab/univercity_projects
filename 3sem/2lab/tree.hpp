#ifndef TREE_HPP
#define TREE_HPP

#include <vector>
#include <iostream>
#include <stack>
#include <string>
#include <queue>
#include <utility>
#include "errors.h"

template<typename K, typename V>
class BinaryTree {
private: 
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;

        Node(const K& k, const V& v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node* root;
    size_t treeSize;

    std::pair<Node*, Node*> findNodeAndParent(const K& key, const V& value) {
        Node* current = root;
        Node* parent = nullptr;
        
        while (current) {
            if (current->key == key && current->value == value) {
                return std::make_pair(current, parent);
            }
            
            parent = current;
            if (key <= current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        
        return std::make_pair(nullptr, nullptr); 
    }

    std::pair<Node*, Node*> findMinNodeAndParent(Node* start) {
        if (!start) return std::make_pair(nullptr, nullptr);
        
        Node* current = start;
        Node* parent = nullptr;
        
        while (current->left) {
            parent = current;
            current = current->left;
        }
        
        return std::make_pair(current, parent);
    }

    Node* copyTree(Node* otherRoot) {
        if (otherRoot == nullptr){
            return nullptr; 
        }
        
        std::stack<Node*> srcStack;
        std::stack<Node*> dstStack;

        Node* newRoot = new Node(otherRoot->key, otherRoot->value); 
        srcStack.push(otherRoot);
        dstStack.push(newRoot);
    
        while (!srcStack.empty()) {
            Node* srcCurrent = srcStack.top();
            Node* dstCurrent = dstStack.top();
            srcStack.pop();
            dstStack.pop();
            
            if (srcCurrent->right) {
                dstCurrent->right = new Node(srcCurrent->right->key, srcCurrent->right->value);
                srcStack.push(srcCurrent->right);
                dstStack.push(dstCurrent->right);
            }
            
            if (srcCurrent->left) {
                dstCurrent->left = new Node(srcCurrent->left->key, srcCurrent->left->value);
                srcStack.push(srcCurrent->left);
                dstStack.push(dstCurrent->left);
            }
        }
        return newRoot;
    }

public:
    BinaryTree() : root(nullptr), treeSize(0) {}

    ~BinaryTree() {
        clear();
    }

    void clear() {
       if (root == nullptr){
        return;
       }
        std::stack<Node*> nodeStack;
        nodeStack.push(root);
        
        while (!nodeStack.empty()) {
            Node* current = nodeStack.top();
            nodeStack.pop();
            
            if (current->left) {
                nodeStack.push(current->left);
            }
            if (current->right) {
                nodeStack.push(current->right);
            }
            
            delete current;
        }
    
        root = nullptr;
        treeSize = 0;
    }

    void insert(const K& key, const V& value) {
        Node* newNode = new Node(key, value);

        if (!root) {
            root = newNode;
            treeSize = 1;
            return;
        }

        Node* current = root;
        Node* parent = nullptr;

        while (current) {
            parent = current;

            if (key <= current->key) {
                current = current->left;
            } else if (key > current->key) {
                current = current->right;
            } 
        }
        
        if (key <= parent->key) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
        treeSize++;
    }

    V* find(const K& key){  
        Node* current = root;

        while (current) {
            if (current->key == key) {
                return &(current->value);
            } else if (key <= current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        
        return nullptr;
    }

    const V* find(const K& key) const {
        const Node* current = root;

        while (current) {
            if (current->key == key) {
                return &(current->value);
            } else if (key <= current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        
        return nullptr;
    }

    bool findKey(const K& key) const{
        return find(key) != nullptr; 
    }

    void remove(const K& key, const V& value) {
        std::pair<Node*, Node*> nodePair = findNodeAndParent(key, value);
        Node* nodeDeleted = nodePair.first;
        Node* parent = nodePair.second;

        if (!nodeDeleted) {
            errorsDetection(Error::KEY_NOT_FOUND);
            return;  
        }

        if (!nodeDeleted->left && !nodeDeleted->right) {
            if (!parent) {
                root = nullptr;
            } else if (parent->left == nodeDeleted) {
                parent->left = nullptr; 
            } else {
                parent->right = nullptr;
            }
            delete nodeDeleted;
            treeSize--;
        }
        else if (!nodeDeleted->left || !nodeDeleted->right) {
            Node* child = nodeDeleted->left ? nodeDeleted->left : nodeDeleted->right;
            
            if (!parent) {
                root = child;
            } else if (parent->left == nodeDeleted) {
                parent->left = child; 
            } else {
                parent->right = child;
            }
            delete nodeDeleted;
            treeSize--;
        }
        else {
            std::pair<Node*, Node*> minPair = findMinNodeAndParent(nodeDeleted->right);
            Node* minNode = minPair.first;
            Node* minParent = minPair.second;

            K minKey = minNode->key;
            V minValue = minNode->value;

            Node* minChild = minNode->right;

            if (minParent) {
                minParent->left = minChild;
            } else {
                nodeDeleted->right = minChild; 
            }
            delete minNode;

            nodeDeleted->key = minKey;
            nodeDeleted->value = minValue;
        }
    }
    
    bool empty() const{
        return root == nullptr;
    }
    
    size_t size() const {
        return treeSize;
    }

    std::vector<K> keys() const{
        std::vector<K> result;
        if (root == nullptr){
            return result;
        }
        std::stack<Node*> nodeStack;
        Node* current = root;
        
        while (current != nullptr || !nodeStack.empty()) {
            while (current != nullptr) {
                nodeStack.push(current);
                current = current->left;
            }
            current = nodeStack.top();
            nodeStack.pop();
            
            result.push_back(current->key);
            
            current = current->right;
        }
        return result;
    }
    
    
    void printTree() const {
        if (root == nullptr) {
            return;
        }
        std::stack<std::pair<Node*, int>> nodeStack;
        nodeStack.push(std::make_pair(root, 0));
        
        while (!nodeStack.empty()) {
            Node* currentNode = nodeStack.top().first;
            int depth = nodeStack.top().second;
            nodeStack.pop();

            for (int i = 0; i < depth; ++i) {
                std::cout << "  ";
            }
            
            std::cout << "[" << currentNode->key << ":" << currentNode->value << "]" << std::endl;
            
            if (currentNode->right != nullptr) {
                nodeStack.push(std::make_pair(currentNode->right, depth + 1));
            }
            if (currentNode->left != nullptr) {
                nodeStack.push(std::make_pair(currentNode->left, depth + 1));
            }
        }
    }
};

#endif