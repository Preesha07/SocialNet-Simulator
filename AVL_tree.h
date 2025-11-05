#pragma once
#include <algorithm>
#include <vector>
#include <utility>
#include <string>
#include <stack>

struct Node {
    std::pair<time_t,std::string> key;
    Node* left;
    Node* right;
    int height;

    Node(std::pair<time_t, std::string> key) : key(key), left(nullptr), right(nullptr), height(0) {}
};


class AVLTree {
private:
    Node* root;

    int height(Node* N) {
        if (N == nullptr)
            return -1;
        return N->height;
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;

        // Return new root
        return x;
    }

    
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;

        // Return new root
        return y;
    }

    int getBalance(Node* N) {
        if (N == nullptr)
            return 0;
        return height(N->left) - height(N->right);
    }

    
    Node* insertHelper(Node* node, std::pair<time_t,std::string> key) {
        // 1. Standard BST insertion
        if (node == nullptr)
            return new Node(key);

        if (key.first < node->key.first)
            node->left = insertHelper(node->left, key);
        else if (key.first > node->key.first)
            node->right = insertHelper(node->right, key);
        else // Duplicate keys are not allowed
            return node;

        // 2. Update height of this ancestor node
        node->height = 1 + std::max(height(node->left), height(node->right));

        // 3. Get the balance factor to check for imbalance
        int balance = getBalance(node);

        // 4. If unbalanced, perform rotations
        // Left Left Case
        if (balance > 1 && key.first < node->left->key.first)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && key.first > node->right->key.first)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && key.first > node->left->key.first) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && key.first < node->right->key.first) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }
    
    
    void deleteTree(Node* node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    
    AVLTree() {
        root = nullptr;
    }

    
    ~AVLTree() {
        deleteTree(root);
    }

    
    void insert(std::pair<time_t,std::string> key) {
        root = insertHelper(root, key);
    }

    std::vector <std::string> Ngreatest(int N)
    {
        std::stack <Node*> tv;
        std::vector <std::string> trav;
        Node* curr=root;
        int count=0;
        if (N==0)
        return trav;

        while(curr!=nullptr || !tv.empty())
        {
            while(curr!=nullptr)
            {
                tv.push(curr);
                curr=curr->right;
            }

            if(!tv.empty())
            {
            curr=tv.top();
            trav.push_back(curr->key.second);
            count++;
            if(count==N)
            {
                return trav;
            }
            tv.pop();
            curr=curr->left;
            }
            
        }

        return trav;
    }
    
};
