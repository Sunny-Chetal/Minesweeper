//
// Created by Sanaa Chetal on 11/26/25.
//

#ifndef MINESWEEPER_GAMESCREEN_H
#define MINESWEEPER_GAMESCREEN_H
#include <map>
#include <vector>
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "Leaderboard.h"
using namespace std;

class TextureManager {
private:
    map<string, sf::Texture*> textures;
public:
    TextureManager();
    ~TextureManager();
    sf::Texture* getTexture(string filename);
};

class Button {
private:
    vector<sf::Sprite> Sprites;
    sf::Vector2f position;
    string text;
public:
    Button(TextureManager* manager, string filename, float x, float y, string text);
    void draw(sf::RenderWindow* window);
    bool isClicked(float x, float y);
    string getText();
    void setTexture(TextureManager* manager, string filename);
};

class Tile {
private:
    bool isMine = false;
    bool revealed = false;
    bool isFlagged = false;
    bool isHidden = false;
    bool minesHidden = true;
    int adjacentMines = 0;
    vector<Tile*> adjacentTiles;
    vector<sf::Sprite> Sprites;
    TextureManager* textureManager;
    sf::Vector2f position;
public:
    Tile(TextureManager* manager);
    void setPosition(float x, float y);
    void setMine(bool isMine);
    void addNewAdjacent(Tile* tile);
    void calculateAdjacentMines();
    void toggleFlag();
    void toggleHidden();
    void showMine(bool debug);
    void reveal();
    void reset();
    bool getIsMine();
    bool getIsFlagged();
    bool getRevealed();
    int getAdjacentMines();
    vector<Tile*> getAdjacentTiles();
    void draw(sf::RenderWindow* window);
};

class Board {
private:
    int colCount;
    int rowCount;
    int mineCount;
    bool debug = false;
    bool pause = false;
    bool won = false;
    bool lost = false;
    vector<Tile*> tiles;
    TextureManager* textureManagerB;
    sf::RenderWindow* window;

public:
    Board(int colCount, int rowCount, int mineCount, sf::RenderWindow* window);
    ~Board();
    void reset();
    void generateTiles();
    void assignAdjacent();
    void placeMines();
    void calculateAdjacents();
    void LeftClick(int mousex, int mousey);
    void RightClick(int mousex, int mousey);
    Tile* getTileAt(int x, int y);
    void revealTile(Tile* tile);
    bool checkWinCondition();
    bool checkLoseCondition();
    void revealAllMines();
    void flagAllMines();
    int getRemainingMines();
    void toggleDebug();
    void togglePause();
    void draw();

};

class Timer {
private:
    bool running = false;
    int storedTime = 0;
    chrono::high_resolution_clock::time_point startTime;

public:
    Timer();
    void start();
    void stop();
    void reset();
    int getTime();
};

class GameScreen {
private:
    int colCount;
    int rowCount;
    int mineCount;
    string playerName;
    sf::RenderWindow* window;
    TextureManager* textureManager;
    Board* board;
    vector<Button> buttons;
    Timer timer;
    bool gameOver = false;
    bool paused = false;
public:
    GameScreen(int colCount, int rowCount, int mineCount, string playerName, sf::RenderWindow* window);
    ~GameScreen();
    void setUpButtons(int colCount, int rowCount);
    void run();
    void handleMouseEvents(const sf::Event& event);
    bool checkConditions();
    void fixData();
    void buttonPressed(string buttonName);
    void drawOneDigit(int number, float x, float y);
    void drawMineCounter();
    void drawTimer();
    void draw();





};


#endif //MINESWEEPER_GAMESCREEN_H