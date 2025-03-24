//Waseem Zahid
//Faizan Niaz
//Muhammad Khan

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include"AVLTree.h"
#include"Player.h"
#include"Enemy.h"
#include <cmath>
#include <queue>

using namespace std;
const int HEIGHT = 30;
const int SIZE = 20;
int progress = 0;
int* gameMap = new int[WIDTH * HEIGHT]();
sf::RectangleShape* displayRects = new sf::RectangleShape[WIDTH * HEIGHT];

string inventoryItems = "Choose \n\n 1- & 10 \n 2- @ 05 \n 3- $ 20 \n 4- % 10 \n 5- # 05 ";

///////////////////////////////////////////////////////////////////////////////////////////////////
class MazeGenerator 
{
public:
    sf::RectangleShape* displayRects;
    int* gameMap;
    sf::CircleShape opponentSprites[10];
    sf::CircleShape playerSprite;
    sf::RenderWindow& window;
    random_device rd;
    mt19937 gen;
    sf::Vector2i startPos, endPos;
    vector<sf::Vector2i> shortestPath;
    sf::Vector2i position;
    AVLTree treasureTree;
    vector<string> treasureDescriptions;

    ///////////////////////////////////////////////////////////////////////////////////////////////////
public:
    MazeGenerator(sf::RenderWindow& w) : window(w), gen(rd()), treasureTree(w, gameMap, WIDTH, HEIGHT)
    {
        displayRects = new sf::RectangleShape[WIDTH * HEIGHT];
        gameMap = new int[WIDTH * HEIGHT]();
        treasureDescriptions =
        {
            "Rusty Sword",
            "Healing Potion",
            "Ancient Tome",
            "Golden Chalice",
            "Diamond Ring"
        };
        generateMaze();
        placePlayer();
        placeEnemy(10);
        placeEndPoint();
        playerSprite.setRadius(CELL_SIZE / 2);
        playerSprite.setFillColor(sf::Color::Cyan);
        for (int i = 0; i < 10; ++i)
        {
            opponentSprites[i].setRadius(CELL_SIZE / 2);
            opponentSprites[i].setFillColor(sf::Color::Green);
        }
        bool playerPlaced = false;
        while (!playerPlaced)
        {
            int x = rand() % WIDTH;
            int y = rand() % HEIGHT;
            if (gameMap[y * WIDTH + x] == 0 && (x != endPos.x || y != endPos.y)) {
                playerSprite.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                position = sf::Vector2i(x, y);
                playerPlaced = true;
            }
        }
        shortestPath = findShortestPath(startPos, endPos);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    ~MazeGenerator() 
    {
        delete[] displayRects;
        delete[] gameMap;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void generateMaze()
    {
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++) 
            {

                displayRects[i + j * HEIGHT].setPosition(i * CELL_SIZE, j * CELL_SIZE);
                displayRects[i + j * HEIGHT].setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                displayRects[i + j * HEIGHT].setOutlineThickness(10.f);
                displayRects[i + j * HEIGHT].setOutlineColor(sf::Color::White);

                if (rand() / (float)RAND_MAX < 0.22f || i == 0 || j == 0 || i == WIDTH - 1 || j == HEIGHT - 1)
                {
                    gameMap[i + j * HEIGHT] = 1;
                    displayRects[i + j * HEIGHT].setFillColor(sf::Color(0, 0, 0));
                }
                else
                {
                    gameMap[i + j * HEIGHT] = 0;
                    displayRects[i + j * HEIGHT].setFillColor(sf::Color::White);
                }
            }
        }
        sf::Vector2i start(1, 1); 
        sf::Vector2i end(WIDTH - 2, HEIGHT - 2); 
        vector<sf::Vector2i> shortestPath = findShortestPath(start, end);

        for (const auto& pos : shortestPath) {
            displayRects[pos.x + pos.y * HEIGHT].setFillColor(sf::Color::Blue);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void placePlayer() 
    {
        startPos = sf::Vector2i(1, 1);
        playerSprite.setRadius(CELL_SIZE / 2);
        playerSprite.setPosition(startPos.x * CELL_SIZE, startPos.y * CELL_SIZE);
        playerSprite.setFillColor(sf::Color::Cyan);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void placeEndPoint()
    {
        endPos = sf::Vector2i(WIDTH - 0, HEIGHT - 0);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void movePlayer(char direction) 
    {
        sf::Vector2i newPosition = position;
        switch (direction) 
        {
        case 'w':
            newPosition.y -= 1;
            break;
        case 'a':
            newPosition.x -= 1;
            break;
        case 's':
            newPosition.y += 1;
            break;
        case 'd':
            newPosition.x += 1;
            break;
        default:
            break;
        }

        if (newPosition.x >= 0 && newPosition.x < WIDTH && newPosition.y >= 0 && newPosition.y < HEIGHT &&
            gameMap[newPosition.y * WIDTH + newPosition.x] == 0)
        {
            playerSprite.setPosition(newPosition.x * CELL_SIZE, newPosition.y * CELL_SIZE);
            position = newPosition;
           
            if (newPosition.x== endPos.x-5 && newPosition.y== endPos.y-5)
            {
                displayWinMessage();
            }

            if (progress >= 50) 
            {
                displayWinMessage();
            }
        }
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////
    void displayWinMessage() 
    {
        sf::RenderWindow winWindow(sf::VideoMode(400, 200), "Congratulations!");

        sf::Font font;
        if (!font.loadFromFile("Font.ttf")) {
            cout << "Failed to load font" << endl;
            return;
        }

        sf::Text winText("Congratulations :) \n \n You Win!", font, 30);
        winText.setPosition(50, 50);

        while (winWindow.isOpen()) 
        {
            sf::Event winEvent;
            while (winWindow.pollEvent(winEvent)) 
            {
                if (winEvent.type == sf::Event::Closed) 
                {
                    winWindow.close();
                }
            }

            winWindow.clear(sf::Color::Magenta);
            winWindow.draw(winText);
            winWindow.display();
            exit(0);
        }
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void drawMaze(sf::RenderWindow& window) 
    {
        for (int i = 0; i < 26 * 30; ++i) 
        {
            window.draw(displayRects[i]);
        }
        window.draw(playerSprite);
        treasureTree.displayTreasureRecursive(treasureTree.root);

        for (int i = 0; i < 10; ++i)
        {
            window.draw(opponentSprites[i]);
        }
        sf::RectangleShape pathRect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        pathRect.setFillColor(sf::Color::Green);
        for (const auto& pos : shortestPath) 
        {
            pathRect.setPosition(pos.x * CELL_SIZE, pos.y * CELL_SIZE);
            window.draw(pathRect);
        }

        sf::Font font;
        if (!font.loadFromFile("Font.ttf")) 
        {
            cout << "Failed to load font file!" << endl;
            return;
        }

        float inventoryTextX = 0;
        float inventoryTextY = (HEIGHT - 1) * CELL_SIZE - 40;

        float progressTextX = (WIDTH - 1) * CELL_SIZE - 120;
        float progressTextY = (HEIGHT - 1) * CELL_SIZE - 40;

        sf::Text inventoryText1("Inventory  [                    ]", font, 20);
        inventoryText1.setFillColor(sf::Color::Blue);
        inventoryText1.setPosition(inventoryTextX, inventoryTextY);
        sf::Text inventoryText2("                         % & ! $ # ", font, 20);
        inventoryText2.setFillColor(sf::Color::Magenta);
        inventoryText2.setPosition(inventoryTextX, inventoryTextY);

        sf::Text progressText("Progress ", font, 20);
        progressText.setFillColor(sf::Color::Blue);
        progressText.setPosition(progressTextX, progressTextY);
        string progressString = "Progress: " + to_string(progress) + "%";
        progressText.setString(progressString);

        window.draw(inventoryText1);
        window.draw(inventoryText2);
        window.draw(progressText);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    vector<sf::Vector2i> findShortestPath(const sf::Vector2i& start, const sf::Vector2i& end)
    {
        vector<sf::Vector2i> path;

        vector<vector<bool>> visited(HEIGHT, vector<bool>(WIDTH, false));
        vector<vector<sf::Vector2i>> parent(HEIGHT, vector<sf::Vector2i>(WIDTH, { -1, -1 }));

        queue<sf::Vector2i> q;
        q.push(start);
        visited[start.y][start.x] = true;

        while (!q.empty()) 
        {
            sf::Vector2i curr = q.front();
            q.pop();

            if (curr == end) 
            {
                // Reconstruct path
                sf::Vector2i p = end;
                while (p != sf::Vector2i(-1, -1)) 
                {
                    path.push_back(p);
                    p = parent[p.y][p.x];
                }
                reverse(path.begin(), path.end());
                return path;
            }

            static const int dx[] = { -1, 0, 1, 0 };
            static const int dy[] = { 0, 1, 0, -1 };

            for (int i = 0; i < 4; i++)
            {
                int newX = curr.x + dx[i];
                int newY = curr.y + dy[i];

                if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT && !visited[newY][newX] && gameMap[newY * WIDTH + newX] == 0)
                {
                    visited[newY][newX] = true;
                    parent[newY][newX] = curr;
                    q.push(sf::Vector2i(newX, newY));
                }
            }
        }

        return path;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void enemyMove() 
    {
        char directions[10];

        for (int i = 0; i < 10; ++i)
        {
            switch (rand() % 4) 
            {
            case 0:
                directions[i] = 'w'; // Up
                break;
            case 1:
                directions[i] = 'a'; // Left
                break;
            case 2:
                directions[i] = 's'; // Down
                break;
            case 3:
                directions[i] = 'd'; // Right
                break;
            }
        }

        for (int i = 0; i < 10; ++i)
        {
            sf::Vector2f currentPosition = opponentSprites[i].getPosition();
            sf::Vector2i newPosition(currentPosition.x / CELL_SIZE, currentPosition.y / CELL_SIZE);

            switch (directions[i]) 
            {
            case 'w':
                newPosition.y -= 1; // Up
                break;
            case 'a':
                newPosition.x -= 1; // Left
                break;
            case 's':
                newPosition.y += 1; // Down
                break;
            case 'd':
                newPosition.x += 1; // Right
                break;
            }

            if (newPosition.x >= 0 && newPosition.x < WIDTH && newPosition.y >= 0 && newPosition.y < HEIGHT &&
                gameMap[newPosition.y * WIDTH + newPosition.x] == 0)
            {
                opponentSprites[i].setPosition(newPosition.x * CELL_SIZE, newPosition.y * CELL_SIZE);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    void placeEnemy(int P) 
    {
        for (int i = 0; i < P; ++i)
        {
            opponentSprites[i].setRadius(10.f);
            opponentSprites[i].setFillColor(sf::Color::Red);
            opponentSprites[i].setPosition(rand() % (WIDTH * CELL_SIZE), rand() % (HEIGHT * CELL_SIZE));
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    bool askToOpenInventory() 
    {
        sf::RenderWindow promptWindow(sf::VideoMode(300, 100), "Inventory Prompt");

        sf::Font font;
        if (!font.loadFromFile("Font.ttf")) 
        {
            cout << "Failed to load font" << endl;
            return false;
        }

        sf::Text promptText("Do you want to open the inventory?", font, 20);
        promptText.setPosition(10, 10);
        promptText.setFillColor(sf::Color::White);

        sf::Text yesText("Yes (Y)", font, 20);
        yesText.setPosition(50, 50);
        yesText.setFillColor(sf::Color::Green);

        sf::Text noText("No (N)", font, 20);
        noText.setPosition(200, 50);
        noText.setFillColor(sf::Color::Red);

        while (promptWindow.isOpen()) 
        {
            sf::Event event;
            while (promptWindow.pollEvent(event)) 
            {
                if (event.type == sf::Event::Closed) 
                {
                    promptWindow.close();
                    return false;
                }
                if (event.type == sf::Event::KeyPressed) 
                {
                    if (event.key.code == sf::Keyboard::Y) 
                    {
                        promptWindow.close();
                        return true;
                    }
                    if (event.key.code == sf::Keyboard::N) 
                    {
                        promptWindow.close();
                        return false;
                    }
                }
            }
            promptWindow.clear(sf::Color::Black);
            promptWindow.draw(promptText);
            promptWindow.draw(yesText);
            promptWindow.draw(noText);
            promptWindow.display();
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void mainLoop() 
    {
        sf::RenderWindow window(sf::VideoMode(CELL_SIZE * WIDTH, CELL_SIZE * HEIGHT), "Maze Game");
        T:
        while (window.isOpen()) 
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) 
                {
                    window.close();
                }
                else if (event.type == sf::Event::KeyPressed) 
                {
                    switch (event.key.code)
                    {
                    case sf::Keyboard::W:
                        movePlayer('w');
                        break;
                    case sf::Keyboard::A:
                        movePlayer('a');
                        break;
                    case sf::Keyboard::S:
                        movePlayer('s');
                        break;
                    case sf::Keyboard::D:
                        movePlayer('d');
                        break;
                    case sf::Keyboard::I: // Added to check for inventory key
                        bool enemyNearby = true;
                        if (enemyNearby)
                        {
                            bool returnToGame = false;
                            if (askToOpenInventory())
                            {
                                sf::RenderWindow inventoryWindow(sf::VideoMode(400, 300), "Inventory Box");
                                sf::Font font;
                                if (!font.loadFromFile("Font.ttf"))
                                {
                                    cout << "Failed to load font" << endl;
                                    return;
                                }
                                sf::Text inventoryText("Inventory", font, 24);
                                inventoryText.setFillColor(sf::Color::Magenta);
                                inventoryText.setPosition(10, 10);

                                inventoryText.setString(inventoryItems);

                                while (inventoryWindow.isOpen() && !returnToGame)
                                {
                                    sf::Event event;
                                    while (inventoryWindow.pollEvent(event))
                                    {
                                        if (event.type == sf::Event::Closed)
                                        {
                                            inventoryWindow.close();
                                        }
                                        // Check for key press events to select an item
                                        if (event.type == sf::Event::KeyPressed)
                                        {
                                            switch (event.key.code)
                                            {
                                            case sf::Keyboard::Num1: 
                                                returnToGame = true;
                                                progress += 10;
                                                inventoryItems = "Choose \n\n 2- @ 05 \n 3- $ 20 \n 4- % 10 \n 5- # 05 ";
                                                placeEnemy(8);
                                                goto T;
                                                break;
                                            case sf::Keyboard::Num2: 
                                                returnToGame = true;
                                                progress += 5;
                                                 inventoryItems = "Choose \n\n 3- $ 20 \n 4- % 10 \n 5- # 05 ";
                                                 placeEnemy(7);
                                                goto T;
                                                break;
                                            case sf::Keyboard::Num3:
                                                returnToGame = true;
                                                progress += 20;
                                                inventoryItems = "Choose \n\n 4- % 10 \n 5- # 05 ";
                                                placeEnemy(3);
                                                goto T;
                                                break;
                                            case sf::Keyboard::Num4: 
                                                returnToGame = true;
                                                progress += 10;
                                                inventoryItems = "Choose \n\n 5- # 05 ";
                                                placeEnemy(1);
                                                goto T;
                                                break;
                                            case sf::Keyboard::Num5:
                                                returnToGame = true;
                                                progress += 5;
                                                inventoryItems = "Empty ";
                                                placeEnemy(0);
                                                goto T;
                                                break;

                                            }
                                        }
                                    }
                                    inventoryWindow.clear(sf::Color::Black);
                                    inventoryWindow.draw(inventoryText);
                                    inventoryWindow.display();
                                }
                            }
                        }                     
                    }
                }
            }

            enemyMove();
            for (int i = 0; i < 10; ++i) 
            {
                if (opponentSprites[i].getGlobalBounds().intersects(playerSprite.getGlobalBounds()))
                {
                    // Collision detected, display "Game Over" and "You Lost" message
                    sf::RenderWindow gameOverWindow(sf::VideoMode(400, 200), "Game Over");

                    sf::Font font;
                    if (!font.loadFromFile("Font.ttf")) 
                    {
                        cout << "Failed to load font" << endl;
                        return;
                    }
                    sf::Text gameOverText("Game Over", font, 30);
                    gameOverText.setPosition(150, 50);

                    sf::Text lostText("You Lost", font, 30);
                    lostText.setPosition(170, 100);

                    while (gameOverWindow.isOpen()) 
                    {
                        sf::Event gameOverEvent;
                        while (gameOverWindow.pollEvent(gameOverEvent))
                        {
                            if (gameOverEvent.type == sf::Event::Closed)
                            {
                                gameOverWindow.close();
                                window.close();
                            }
                        }
                        gameOverWindow.clear(sf::Color::Red);
                        gameOverWindow.draw(gameOverText);
                        gameOverWindow.draw(lostText);
                        gameOverWindow.display();
                        exit(0);
                    }
                }
            }
            window.clear();
            drawMaze(window);
            window.display();
        }
        enemyMove();
    }
};
