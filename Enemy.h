//Waseem Zahid
//Faizan Niaz
//Muhammad Khan

#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include"AVLTree.h"
#include"Maze.h"
#include <queue>
using namespace std;

const int WIDTH = 30;
const int Height = 30;

class Enemy
{
private:
    sf::Vector2i position;
    sf::Color color;
    std::mt19937 rng; // Random number generator
   


    sf::Vector2i getPosition() const
    {
        return position;
    }

    void SetPosition(sf::Vector2i newPos) 
    {
        position = newPos;
    }

    void setColor(sf::Color newColor) 
    {
        color = newColor;
    }

    sf::Color getColor() const 
    {
        return color;
    }


};

#endif // ENEMY_H

