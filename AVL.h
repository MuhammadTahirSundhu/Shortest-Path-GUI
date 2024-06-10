#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <algorithm>
#include <time.h>
using namespace std;
using namespace sf;

class Inventory {
public:
    Texture tex;
    Sprite  sprite;
    int val;
    char identifier;
    Inventory* left;
    Inventory* right;
    int height;

   
    Inventory(string file)
    {
        tex.loadFromFile(file);
        sprite.setTexture(tex);
        val = 0;
        left = nullptr;
        right = nullptr;
        height = 0;
        identifier = ' ';
    }
    
};

int height(Inventory* ptr)
{
    if (ptr == NULL)
        return 0;
    else
        return ptr->height;
}
int max(int a, int b)
{
    if (a > b) return a;
    else return b;
}
int BalanceFactor(Inventory* ptr)
{
    if (ptr == nullptr)
        return 0;
    else
        return height(ptr->left) - height(ptr->right);
}
Inventory* rightRotate(Inventory* y)
{
    Inventory* x = y->left;
    Inventory* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}
Inventory* leftRotate(Inventory* y)
{
    Inventory* x = y->right;
    Inventory* T2 = x->left;
    x->left = y;
    y->right = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}
Inventory* minValueNode(Inventory* node)
{
    Inventory* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

Inventory* insert(Inventory* node, int key,string file,int x,int y,char id)
{
    if (node == nullptr)
    {
        node = new Inventory(file);

        float scaleX = 20.0f / node->sprite.getGlobalBounds().width;
        float scaleY = 20.0f / node->sprite.getGlobalBounds().height;
        node->sprite.setScale(scaleX-0.1, scaleY-0.1);       
        node->sprite.setPosition(x, y);
        node->val = key;
        node->height = 1;
        node->identifier = id;
        return node;
    }
    else if (key > node->val)
        node->right = insert(node->right, key, file,x,y,id);
    else if (key < node->val)
        node->left = insert(node->left, key, file,x,y,id);

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = BalanceFactor(node);

    if (balance > 1 && key < node->left->val)
        node = rightRotate(node);

    else if (balance > 1 && key > node->left->val)
    {
        node->left = leftRotate(node->left);
        node = rightRotate(node);
    }
    else if (balance < -1 && key > node->right->val)
        node = leftRotate(node);

    else if (balance < -1 && key < node->right->val)
    {
        node->right = rightRotate(node->right);
        node = leftRotate(node);
    }

    return node;
}
Inventory* deleteN(Inventory*& root, int key)
{
    if (root == NULL)
        return root;
    if (key < root->val)
        root->left = deleteN(root->left, key);
    else if (key > root->val)
        root->right = deleteN(root->right, key);
    else
    {
        if ((root->left == NULL) ||
            (root->right == NULL))
        {
            Inventory* temp = root->left ?
                root->left :
                root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;
            free(temp);
        }
        else
        {
            Inventory* temp = minValueNode(root->right);
            root->val = temp->val;
            root->right = deleteN(root->right,
                temp->val);
        }
    }

    if (root == NULL)
        return root;
    root->height = 1 + max(height(root->left),
        height(root->right));

    int balance = BalanceFactor(root);

    if (balance > 1 &&
        BalanceFactor(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 &&
        BalanceFactor(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 &&
        BalanceFactor(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 &&
        BalanceFactor(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}
Inventory* retrieve(Inventory* root, int key)
{
    Inventory* temp = root;
    while (temp)
    {
        if (temp->val == key)
            return root;
        else if (key < temp->val)
            temp = temp->left;
        else
            temp = temp->right;
    }
    return NULL;
}

void PreOrder(Inventory* root,sf::RenderWindow &window)
{
    if (root != NULL)
    {
        window.draw(root->sprite);
        PreOrder(root->left, window);
        PreOrder(root->right, window);
    }
}
