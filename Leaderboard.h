//
// Created by Sanaa Chetal on 11/19/25.
//

#ifndef MINESWEEPER_LEADERBOARD_H
#define MINESWEEPER_LEADERBOARD_H
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
using namespace std;

class Leaderboard {
    private:
    int width, height;
    vector<string> times;
    vector<string> names;
    sf::RenderWindow* window;

    public:
    Leaderboard(unsigned int width, unsigned int height, sf::RenderWindow* window);
    void pullData();
    void setText(sf::Text& text, float x, float y);
    void writeData();
};

#endif //MINESWEEPER_LEADERBOARD_H