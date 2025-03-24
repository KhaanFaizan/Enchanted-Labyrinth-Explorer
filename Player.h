//Waseem Zahid
//Faizan Niaz
//Muhammad Khan

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <cmath>
#include"AVLTree.h"
#include"Maze.h"
#include"Enemy.h"
#include <vector>
#include <queue>

using namespace std;

class Player 
{
private:
    sf::Vector2i position;
    sf::CircleShape playerSprite; 
    sf::Color color;
    int treasuresCollected;

public:
    Player(sf::Vector2i startPos, sf::Color playerColor) : position(startPos), color(playerColor), treasuresCollected(0) {}

    void placePlayer() 
    {
      
        sf::Vector2i startPos(1, 1); 

        playerSprite.setRadius(CELL_SIZE / 2);
        playerSprite.setPosition(startPos.x * CELL_SIZE, startPos.y * CELL_SIZE);

        playerSprite.setFillColor(sf::Color::Cyan);

        setPosition(startPos);
    }

    void movePlayer(char direction) 
    {
        // Move the player in the specified direction
        switch (direction) 
        {
        case 'w': // Move up
            position.y -= 1;
            break;
        case 'a': // Move left
            position.x -= 1;
            break;
        case 's': // Move down
            position.y += 1;
            break;
        case 'd': // Move right
            position.x += 1;
            break;
        default:
            // Invalid direction, do nothing
            break;
        }

        playerSprite.setPosition(position.x * CELL_SIZE, position.y * CELL_SIZE);
    }

    void setPosition(sf::Vector2i newPos) 
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

    void collectTreasure()
    {
        treasuresCollected++;
    }

    int getTreasuresCollected() const 
    {
        return treasuresCollected;
    }
};
