//Waseem Zahid
//Faizan Niaz
//Muhammad Khan

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <cmath>
#include <queue>
using namespace std;

const int CELL_SIZE = 20;
const int NUM_TREASURES = 10;

struct TreasureItem 
{
    int key;
    string name;
    string description;

    TreasureItem(int k, const string& n, const string& d) : key(k), name(n), description(d) {}
};

struct Node 
{
    TreasureItem item;
    int height;
    Node* left;
    Node* right;

    Node(const TreasureItem& i) : item(i), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree
{
public:
    Node* root;
    sf::RenderWindow& window;
    int* gameMap;
    int mazeWidth;
    int mazeHeight;
    AVLTree(sf::RenderWindow& w, int* map, int width, int height) : root(nullptr), window(w), gameMap(map), mazeWidth(width), mazeHeight(height) {}

    // Helper function to get height of node
    int height(Node* node)
    {
        return node ? node->height : 0;
    }

    // Helper function to get balance factor of node
    int getBalanceFactor(Node* node) 
    {
        return node ? height(node->left) - height(node->right) : 0;
    }

    // Update height
    void updateHeight(Node* node) 
    {
        node->height = 1 + max(height(node->left), height(node->right));
    }

    // Minimum value at node
    Node* minValueNode(Node* node) 
    {
        Node* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    // Rotation functions
    Node* rightRotate(Node* y)
    {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = 1 + max(height(y->left), height(y->right));
        x->height = 1 + max(height(x->left), height(x->right));

        return x;
    }

    Node* leftRotate(Node* x) 
    {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = 1 + max(height(x->left), height(x->right));
        y->height = 1 + max(height(y->left), height(y->right));

        return y;
    }

    // Helper function to insert node into AVL tree
    Node* insert(Node* node, const TreasureItem& item) 
    {
        if (!node)
            return new Node(item);

        if (item.key < node->item.key)
            node->left = insert(node->left, item);
        else if (item.key > node->item.key)
            node->right = insert(node->right, item);
        else
            return node;

        node->height = 1 + max(height(node->left), height(node->right));

        int balanceFactor = getBalanceFactor(node);

        if (balanceFactor > 1) 
        {
            if (item.key < node->left->item.key)
                return rightRotate(node);
            else
            {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }

        if (balanceFactor < -1) 
        {
            if (item.key > node->right->item.key)
                return leftRotate(node);
            else
            {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
        }

        return node;
    }

    Node* deleteNode(Node* root, const int& value)
    {
        if (root == nullptr)
            return root;

        if (value < root->height)
            root->left = deleteNode(root->left, value);
        else if (value > root->height)
            root->right = deleteNode(root->right, value);
        else 
        {
            if (root->left == nullptr || root->right == nullptr)
            {
                Node* temp = root->left ? root->left : root->right;

                if (temp == nullptr) 
                {
                    temp = root;
                    root = nullptr;
                }
                else
                    *root = *temp;

                delete temp;
            }
            else 
            {
                Node* temp = minValueNode(root->right);
                root->height = temp->height;
                root->right = deleteNode(root->right, temp->height);
            }
        }

        if (root == nullptr)
            return root;

        updateHeight(root);
        int balance = getBalanceFactor(root);

        if (balance > 1 && getBalanceFactor(root->left) >= 0)
            return rightRotate(root);

        if (balance > 1 && getBalanceFactor(root->left) < 0)
        {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        if (balance < -1 && getBalanceFactor(root->right) <= 0)
            return leftRotate(root);

        if (balance < -1 && getBalanceFactor(root->right) > 0) 
        {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    void insert(const TreasureItem& item) 
    {
        root = insertRecursive(root, item);
    }

    Node* insertRecursive(Node* node, const TreasureItem& item) 
    {
        return insert(node, item);
    }

    Node* search(int key) 
    {
        return searchRecursive(root, key);
    }

    Node* searchRecursive(Node* node, int key)
    {
        if (node == nullptr || node->item.key == key)
            return node;

        if (key < node->item.key)
            return searchRecursive(node->left, key);
        else
            return searchRecursive(node->right, key);
    }

    void displayTreasureRecursive(Node* node)
    {
        if (node != nullptr) {
            sf::CircleShape coinSprite;
            coinSprite.setRadius(CELL_SIZE / 4);
            coinSprite.setFillColor(sf::Color::Yellow);

            int x = node->item.key % mazeWidth;
            int y = node->item.key / mazeWidth;

            if (gameMap[y * mazeWidth + x] == 0)
            { 
                coinSprite.setPosition(x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2);
                window.draw(coinSprite);
            }

            displayTreasureRecursive(node->left);
            displayTreasureRecursive(node->right);
        }
    }

public:
    void placeTreasures(int startX, int startY, int endX, int endY) 
    {
        string treasureDescriptions[] = 
        {
            "Rusty Sword",
            "Healing Potion",
            "Ancient Tome",
            "Golden Chalice",
            "Diamond Ring"
        };

        for (int i = 0; i < NUM_TREASURES; i++) 
        {
            int treasureKey;
            bool validKey = false;
            while (!validKey) 
            {
                treasureKey = rand() % (mazeWidth * mazeHeight);
                int x = treasureKey % mazeWidth;
                int y = treasureKey / mazeWidth;
                if (gameMap[y * mazeWidth + x] == 0 && treasureKey != startX + startY * mazeWidth && treasureKey != endX + endY * mazeWidth) {
                    validKey = true;
                }
            }
            string treasureName = "Treasure " + to_string(treasureKey);
            string treasureDescription = treasureDescriptions[treasureKey % (sizeof(treasureDescriptions) / sizeof(treasureDescriptions[0]))];
            insert(TreasureItem(treasureKey, treasureName, treasureDescription));
        }
    }
};
