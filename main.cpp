//Waseem Zahid
//Faizan Niaz
//Muhammad Khan

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include"AVLTree.h"
#include"Maze.h"
#include"Player.h"
#include"Enemy.h"
#include <cmath>
#include <queue>
#include <SFML/Audio.hpp>

using namespace std;

int main()
{    L:
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile("E:\\22i-1355_D_A1\\Project2\\backgroundmusic.MP3"))
        {
            cout << "Failed to load sound file!" << endl;
            return 1;
        }

        sf::Sound sound;
        sound.setBuffer(buffer);
        sound.play();

        while (sound.getStatus() == sf::Sound::Playing)
        {

            sf::RenderWindow window(sf::VideoMode(800, 600), "Enchanted Labyrinth Explorer");

            sf::Font font;
            if (!font.loadFromFile("Font.ttf")) {
                cout << "Failed to load font" << endl;
                return 1;
            }

            sf::Text title("Enchanted Labyrinth Explorer", font, 48);
            title.setFillColor(sf::Color::Blue);
            title.setStyle(sf::Text::Bold);
            title.setPosition(50.f, 50.f);

            sf::Text play("1. Game Play", font, 30);
            play.setFillColor(sf::Color::Green);
            play.setPosition(300.f, 200.f);

            sf::Text instruction("2. Instruction", font, 30);
            instruction.setFillColor(sf::Color::Green);
            instruction.setPosition(300.f, 250.f);

            sf::Text rules("3. Rules", font, 30);
            rules.setFillColor(sf::Color::Green);
            rules.setPosition(300.f, 300.f);

            sf::Text groupMembers("4. Group Members", font, 30);
            groupMembers.setFillColor(sf::Color::Green);
            groupMembers.setPosition(300.f, 350.f);

            while (window.isOpen()) 
            {
                sf::Event event;
                while (window.pollEvent(event)) 
                {
                    if (event.type == sf::Event::Closed)
                        window.close();
                    else if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                            if (play.getGlobalBounds().contains(mousePos)) {
                                // Open game play
                                cout << "Opening game play..." << endl;

                                sf::RenderWindow window(sf::VideoMode(CELL_SIZE * WIDTH, CELL_SIZE * HEIGHT), "Maze Game");
                                MazeGenerator generator(window);
                                generator.mainLoop();

                            }
                            else if (instruction.getGlobalBounds().contains(mousePos)) {
                                // Show instructions
                                while (window.isOpen()) {
                                    sf::Event event;
                                    while (window.pollEvent(event)) {
                                        if (event.type == sf::Event::Closed)
                                            window.close();
                                        else if (event.type == sf::Event::MouseButtonPressed) {
                                            if (event.mouseButton.button == sf::Mouse::Left) {
                                                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                                                cout << "Showing instructions..." << endl;
                                                sf::Text instructionsText("\n\n  INSTRUCTION:  \n\n"
                                                    "Welcome to Enchanted Labyrinth Explorer!\n\n"
                                                    "- Use S, W, A and D keys to move the player\n"
                                                    "- Navigate through the maze to find treasures\n"
                                                    "- Maximum Progress 50 %\n"
                                                    "- Avoid enemies\n\n"
                                                    "Have fun exploring!", font, 20);
                                                instructionsText.setFillColor(sf::Color::Red);
                                                instructionsText.setPosition(50.f, 100.f);
                                                window.clear(sf::Color::Black);
                                                window.draw(instructionsText);
                                                window.display();
                                            }
                                        }
                                    }
                                }
                                goto L;
                            }

                            else if (rules.getGlobalBounds().contains(mousePos)) {
                                // Show rules
                                while (window.isOpen()) {
                                    sf::Event event;
                                    while (window.pollEvent(event)) {
                                        if (event.type == sf::Event::Closed)
                                            window.close();
                                        else if (event.type == sf::Event::MouseButtonPressed) {
                                            if (event.mouseButton.button == sf::Mouse::Left) {
                                                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                                                cout << "Showing rules..." << endl;
                                                sf::Text rulesText("\n\n Rules: \n\n"
                                                    "1. Collect all treasures to win\n"
                                                    "2. Avoid contact with enemies\n"
                                                    "3. Use your wit to navigate through the maze\n\n"
                                                    "Good luck!", font, 20);
                                                rulesText.setFillColor(sf::Color::Magenta);
                                                rulesText.setPosition(50.f, 100.f);
                                                window.clear(sf::Color::Black);
                                                window.draw(rulesText);
                                                window.display();
                                            }
                                        }
                                    }
                                }
                                goto L;
                            }

                            else if (groupMembers.getGlobalBounds().contains(mousePos)) {
                                // Show group members
                                while (window.isOpen()) {
                                    sf::Event event;
                                    while (window.pollEvent(event)) {
                                        if (event.type == sf::Event::Closed)
                                            window.close();
                                        else if (event.type == sf::Event::MouseButtonPressed) {
                                            if (event.mouseButton.button == sf::Mouse::Left) {
                                                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                                                cout << "Group Members..." << endl;
                                                sf::Text rulesText("\n\nGroup Members: \n\n"
                                                    "1. Waseem Zahid 22i-1355\n"
                                                    "2. Faizan Niaz 22i-1356\n"
                                                    "3. Muhammad Khan 22i-2604\n\n", font, 20);
                                                rulesText.setFillColor(sf::Color::Cyan);
                                                rulesText.setPosition(50.f, 100.f);
                                                window.clear(sf::Color::Black);
                                                window.draw(rulesText);
                                                window.display();
                                            }
                                        }
                                    }
                                }
                            }
                            goto L;
                        }
                    }
                }

                window.clear(sf::Color::Black);
                window.draw(title);
                window.draw(play);
                window.draw(instruction);
                window.draw(rules);
                window.draw(groupMembers);
                window.display();
            }

       }
    return 0;
}



