//
// Created by Sanaa Chetal on 11/29/25.
//
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Leaderboard.h"
using namespace std;

//
//Leaderboard
//

Leaderboard::Leaderboard(unsigned int width, unsigned int height, sf::RenderWindow* window) {
    this->width = width;
    this->height = height;
    this->window = window;
    pullData();
    writeData();
}

void Leaderboard::pullData() {
    ifstream leaderboardFile("files/leaderboard.txt");
    if (!leaderboardFile.is_open()) {
        cerr << "Could not open file" << endl;
        return;
    }
    times.clear();
    names.clear();
    string line;
    while (getline(leaderboardFile, line)) {
        stringstream ss(line);
        string time;
        string name;
        getline(ss, time, ',');
        getline(ss, name);
        if (name[0] == ' ') {
            name = name.substr(1, name.size() - 1);
        }
        times.push_back(time);
        names.push_back(name);
    }
    leaderboardFile.close();
}

void Leaderboard::setText(sf::Text& text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    sf::Vector2f newOrigin(textRect.position.x + textRect.size.x/2.0f, textRect.position.y + textRect.size.y/2.0f);
    text.setOrigin(newOrigin);
    text.setPosition(sf::Vector2f(x, y));
}

void Leaderboard::writeData() {
    const sf::Font font("files/font.ttf");

    //Leaderboard
    sf::Text leaderboard(font, "LEADERBOARD", 20);
    setText(leaderboard, (width/2), ((height/2) - 120));
    leaderboard.setStyle(sf::Text::Bold);
    leaderboard.setStyle(sf::Text::Underlined);

    //Contents of Leaderboard
    string text = "1.\t" + times[0] + "\t" + names[0] + "\n\n2.\t" + times[1] + "\t" + names[1] +
        "\n\n3.\t" + times[2] + "\t" + names[2] + "\n\n4.\t" + times[3] + "\t" + names[3] +
        "\n\n5.\t" + times[4] + "\t" + names[4];
    sf::Text leaderboardText(font, text, 18);
    setText(leaderboardText, (width/2), ((height/2) + 20));
    leaderboardText.setStyle(sf::Text::Bold);

    while (window->isOpen()) {
        // Process events
        while (const std::optional event = window->pollEvent()) {
            // Close window: exit
            if (event->is<sf::Event::Closed>()) {
                window->close();
            }
            // Clear screen
            window->clear(sf::Color::Blue);
            window->draw(leaderboard);
            window->draw(leaderboardText);
            window->display();
        }
    }
}




