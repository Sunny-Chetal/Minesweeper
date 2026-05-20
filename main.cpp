#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include "GameScreen.h"
using namespace std;

void setText(sf::Text& text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    sf::Vector2f newOrigin(textRect.position.x + textRect.size.x/2.0f, textRect.position.y + textRect.size.y/2.0f);
    text.setOrigin(newOrigin);
    text.setPosition(sf::Vector2f(x, y));
}


 int main() {
    srand(time(0));
    //Read config file
    string path = "files/config.cfg";
    ifstream file;
    file.open(path);
    string line;
    getline(file, line);
    unsigned int colCount = stoi(line);
    getline(file, line);
    unsigned int rowCount = stoi(line);
    getline(file, line);
    unsigned int mineCount = stoi(line);

    unsigned int height = ((rowCount * 32) + 100);
    unsigned int width = (colCount * 32);

    // Create the main window
    sf::RenderWindow window(sf::VideoMode({width, height}), "Minesweeper");

    // Create a font
    const sf::Font font("files/font.ttf");

    //Welcome Screen
    bool welcomeScreen = true;
        //Welcome
    sf::Text welcome(font, "WELCOME TO MINESWEEPER!", 24);
    setText(welcome, (width/2), ((height/2) - 150));
    welcome.setStyle(sf::Text::Bold);
    welcome.setStyle(sf::Text::Underlined);
        //Enter Name
    sf::Text enterName(font, "Enter your name:", 20);
    setText(enterName, (width/2), ((height/2) - 75));
    enterName.setStyle(sf::Text::Bold);
        //User Typed Name
    string nameStr = "";
    int charNum = 0;

    // Start the game loop
    while (window.isOpen()) {
        // Process events
        while (const std::optional event = window.pollEvent()) {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();

            // Clear screen
            window.clear(sf::Color::Blue);

            // Update the window
                if (const auto* written = event->getIf<sf::Event::TextEntered>()) {
                    if (charNum <= 10){
                        if (isalpha(written->unicode)) {
                            if (charNum < 10){
                                char letter = static_cast<char>(written->unicode);
                                if (charNum == 0) {
                                    letter = toupper(letter);
                                }
                                else {
                                    letter = tolower(letter);
                                }
                                charNum++;
                                nameStr += letter;
                            }
                        }
                        else if (written->unicode == 8 && charNum != 0) {
                            charNum--;
                            nameStr.erase(charNum);
                        }
                        else if (written->unicode == 10 && charNum >= 1) {
                            welcomeScreen = false;
                            GameScreen game(colCount, rowCount, mineCount, nameStr, &window);
                            game.run();
                        }
                    }
                }
            // Draw the string
            sf::Text name(font, nameStr + "|", 18);
            setText(name, (width/2), ((height/2) - 45));
            name.setFillColor(sf::Color::Yellow);
            name.setStyle(sf::Text::Bold);

            if (welcomeScreen) {
                window.draw(welcome);
                window.draw(enterName);
                window.draw(name);
                window.display();
            }
        }
    }
    return 0;
}