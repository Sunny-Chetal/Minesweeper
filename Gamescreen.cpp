//
// Created by Sanaa Chetal on 11/26/25.
//
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "GameScreen.h"
using namespace std;

//
//TextureManager
//

TextureManager::TextureManager(){}

TextureManager::~TextureManager() {
    for (auto i = textures.begin(); i != textures.end(); i++) {
        delete i->second;
    }
    textures.clear();
}

sf::Texture* TextureManager::getTexture(string filename) {
    if (textures.find(filename) != textures.end()) {
        return textures[filename];
    }
    sf::Texture* texture = new sf::Texture();
    if (!texture->loadFromFile(filename)) {
        cerr << "Error loading texture " << filename << endl;
        delete texture;
        return nullptr;
    }
    textures[filename] = texture;
    return texture;
}

//
//Button
//

Button::Button(TextureManager* manager, string filename, float x, float y, string text) {
    this->text = text;
    sf::Texture* buttonTexture = manager->getTexture(filename);
    if (!buttonTexture) {
        cerr << "Error loading button " << filename << endl;
        return;
    }
    sf::Sprite buttonSprite(*buttonTexture);
    buttonSprite.setPosition({x, y});
    Sprites.push_back(buttonSprite);
}

void Button::draw(sf::RenderWindow* window) {
    window->draw(Sprites[0]);
}

bool Button::isClicked(float x, float y) {
    return Sprites[0].getGlobalBounds().contains({x,y});
}

string Button::getText() {
    return text;
}

void Button::setTexture(TextureManager* manager, string filename) {
    Sprites[0].setTexture(*manager->getTexture(filename));
}


//
//Tile
//

Tile::Tile(TextureManager* manager) : textureManager(manager) {
    sf::Texture* tile = textureManager->getTexture("files/images/tile_hidden.png");
    if (!tile) {
        cerr << "Failed to load tile texture" << endl;
        return;
    }
    sf::Sprite sprite(*tile);
    sprite.setPosition(this->position);
    sf::Sprite flagSprite(*tile);
    flagSprite.setPosition(this->position);
    sf::Sprite mineSprite(*tile);
    mineSprite.setPosition(this->position);
    sf::Sprite numberSprite(*tile);
    numberSprite.setPosition(this->position);
    Sprites.push_back(sprite);
    Sprites.push_back(flagSprite);
    Sprites.push_back(mineSprite);
    Sprites.push_back(numberSprite);

}

void Tile::setPosition(float x, float y) {
    Sprites[0].setPosition({x, y});
    this->position = sf::Vector2f(x, y);
}


void Tile::setMine(bool isMine) {
    this->isMine = isMine;
}

void Tile::addNewAdjacent(Tile* tile) {
    adjacentTiles.push_back(tile);
}

void Tile::calculateAdjacentMines() {
    adjacentMines = 0;
    for (int i = 0; i < adjacentTiles.size(); i++) {
        if (adjacentTiles[i]->isMine) {
            adjacentMines++;
        }
    }
}

void Tile::toggleFlag() {
    sf::Texture* flag = textureManager->getTexture("files/images/flag.png");
    Sprites[1].setTexture(*flag);
    Sprites[1].setColor(sf::Color::Transparent);
    Sprites[1].setPosition(this->position);
    if (isFlagged && !revealed) {
        isFlagged = false;
        Sprites[1].setColor(sf::Color::Transparent);
    }
    else if (!isFlagged && !revealed) {
        isFlagged = true;
        Sprites[1].setColor(sf::Color::White);
    }
}

void Tile::toggleHidden() {
    if (!isHidden) {
        isHidden = true;
        sf::Texture* reveal = textureManager->getTexture("files/images/tile_revealed.png");
        Sprites[0].setTexture(*reveal);
        Sprites[1].setColor(sf::Color::Transparent);
        Sprites[2].setColor(sf::Color::Transparent);
        Sprites[3].setColor(sf::Color::Transparent);
    }
    else if (isHidden) {
        if (!revealed) {
            isHidden = false;
            sf::Texture* hidden = textureManager->getTexture("files/images/tile_hidden.png");
            Sprites[0].setTexture(*hidden);
            Sprites[1].setColor(sf::Color::White);
            Sprites[2].setColor(sf::Color::White);
            Sprites[3].setColor(sf::Color::White);
        }
        else if (revealed) {
            isHidden = false;
            Sprites[1].setColor(sf::Color::White);
            Sprites[2].setColor(sf::Color::White);
            Sprites[3].setColor(sf::Color::White);
        }
    }
}


void Tile::showMine(bool debug) {
    if (!isMine || revealed || isFlagged) {return;}
    sf::Texture* mine = textureManager->getTexture("files/images/mine.png");
    if (debug) {
        Sprites[2].setTexture(*mine);
        Sprites[2].setColor(sf::Color::White);
        Sprites[2].setPosition(this->position);
    }
    else if (!debug) {
        Sprites[2].setColor(sf::Color::Transparent);
    }
}



void Tile::reveal() {
    if (revealed) {return;}
    revealed = true;

    sf::Texture* reveal = textureManager->getTexture("files/images/tile_revealed.png");
    Sprites[0].setTexture(*reveal);
    Sprites[0].setPosition(this->position);

    if (isMine) {
        sf::Texture* mine = textureManager->getTexture("files/images/mine.png");
        Sprites[2].setTexture(*mine);
        Sprites[2].setPosition(this->position);
        return;
    }
    else if (adjacentMines > 0) {
        if (adjacentMines == 1) {
            sf::Texture* number1 = textureManager->getTexture("files/images/number_1.png");
            Sprites[3].setTexture(*number1);
            Sprites[3].setPosition(this->position);
        }
        else if (adjacentMines == 2) {
            sf::Texture* number2 = textureManager->getTexture("files/images/number_2.png");
            Sprites[3].setTexture(*number2);
            Sprites[3].setPosition(this->position);
        }
        else if (adjacentMines == 3) {
            sf::Texture* number3 = textureManager->getTexture("files/images/number_3.png");
            Sprites[3].setTexture(*number3);
            Sprites[3].setPosition(this->position);
        }
        else if (adjacentMines == 4) {
            sf::Texture* number4 = textureManager->getTexture("files/images/number_4.png");
            Sprites[3].setTexture(*number4);
            Sprites[3].setPosition(this->position);
        }
        else if (adjacentMines == 5) {
            sf::Texture* number5 = textureManager->getTexture("files/images/number_5.png");
            Sprites[3].setTexture(*number5);
            Sprites[3].setPosition(this->position);
        }
        else if (adjacentMines == 6) {
            sf::Texture* number6 = textureManager->getTexture("files/images/number_6.png");
            Sprites[3].setTexture(*number6);
            Sprites[3].setPosition(this->position);
        }
        else if (adjacentMines == 7) {
            sf::Texture* number7 = textureManager->getTexture("files/images/number_7.png");
            Sprites[3].setTexture(*number7);
            Sprites[3].setPosition(this->position);
        }
        else if (adjacentMines == 8) {
            sf::Texture* number8 = textureManager->getTexture("files/images/number_8.png");
            Sprites[3].setTexture(*number8);
            Sprites[3].setPosition(this->position);
        }

    }
}

void Tile::reset() {
    this->revealed = false;
    this->isFlagged = false;
    sf::Texture* tex = textureManager->getTexture("files/images/tile_hidden.png");
    Sprites[0].setTexture(*tex);
    Sprites[1].setTexture(*tex);
    Sprites[2].setTexture(*tex);
    Sprites[3].setTexture(*tex);
}

bool Tile::getIsMine() {
    return isMine;
}

bool Tile::getIsFlagged() {
    return isFlagged;
}

bool Tile::getRevealed() {
    return revealed;
}

int Tile::getAdjacentMines() {
    return adjacentMines;
}

vector<Tile*> Tile::getAdjacentTiles() {
    return adjacentTiles;
}

void Tile::draw(sf::RenderWindow* window) {
    window->draw(Sprites[0]);
    window->draw(Sprites[2]);
    window->draw(Sprites[3]);
    window->draw(Sprites[1]);
}

//
//Board
//

Board::Board(int colCount, int rowCount, int mineCount, sf::RenderWindow* window) {
    this->colCount = colCount;
    this->rowCount = rowCount;
    this->mineCount = mineCount;
    this->window = window;
    textureManagerB = new TextureManager();
    generateTiles();
    placeMines();
    assignAdjacent();
    calculateAdjacents();
}

Board::~Board() {
    for (int i = 0 ; i < rowCount * colCount ; i++) {
        delete tiles[i];
    }
    tiles.clear();
    delete textureManagerB;
}

void Board::reset() {
    for (int i = 0 ; i < rowCount * colCount ; i++) {
        delete tiles[i];
    }
    tiles.clear();
    generateTiles();
    placeMines();
    assignAdjacent();
    calculateAdjacents();
    debug = false;
    pause = false;
    won = false;
    lost = false;
}

void Board::generateTiles() {
    for (int row = 0; row < rowCount; row++) {
        for (int col = 0; col < colCount; col++) {
            Tile* tile = new Tile(textureManagerB);
            float x = col * 32.0f;
            float y = row * 32.0f;
            tile->setPosition(x, y);
            tiles.push_back(tile);
        }

    }
}

void Board::assignAdjacent() {
    for (int row = 0; row < rowCount; row++) {
        for (int col = 0; col < colCount; col++) {
            Tile* currentTile = tiles[row * colCount + col];
            for (int r = row - 1; r <= row + 1; r++) {
                for (int c = col - 1; c <= col + 1; c++) {
                    if (r == row && c == col) {
                        continue;
                    }
                    if (r >= 0 && r < rowCount && c >= 0 && c < colCount) {
                        Tile* nextTile = tiles[r * colCount + c];
                        if (nextTile != nullptr) {
                            currentTile->addNewAdjacent(nextTile);
                        }
                    }
                }
            }
        }
    }
}

void Board::placeMines() {
    int mines = 0;
    while (mines < mineCount) {
        int random = rand() % (rowCount * colCount);
        if (!tiles[random]->getIsMine()) {
            tiles[random]->setMine(true);
            mines++;

        }
    }
}

void Board::calculateAdjacents() {
    for (int i = 0; i < rowCount * colCount; i++) {
        tiles[i]->calculateAdjacentMines();
    }
}

void Board::LeftClick(int mousex, int mousey) {
    Tile* currentTile = getTileAt(mousex, mousey);
    if (currentTile) {
        revealTile(currentTile);
    }
}

void Board::RightClick(int mousex, int mousey) {
    Tile* currentTile = getTileAt(mousex, mousey);
    if (currentTile) {
        currentTile->toggleFlag();
    }
}

Tile* Board::getTileAt(int x, int y) {
    int col = x / 32;
    int row = y / 32;
    if (row < 0 || row >= rowCount || col < 0 || col >= colCount) {
        return nullptr;
    }
    return tiles[row * colCount + col];
}

void Board::revealTile(Tile* tile) {
    if (tile == nullptr || tile->getRevealed() || tile->getIsFlagged()) {return;}
    tile->reveal();
    if (tile->getAdjacentMines() == 0 && tile->getIsMine() == false){
        for (int i = 0; i < tile->getAdjacentTiles().size(); i++) {
            revealTile(tile->getAdjacentTiles()[i]);
        }
    }
}


bool Board::checkWinCondition() {
   for (int i = 0; i < rowCount * colCount; i++) {
       if (!tiles[i]->getIsMine() && !tiles[i]->getRevealed()) {
           return false;
       }
   }
    won = true;
    return true;
}

bool Board::checkLoseCondition() {
    for (int i = 0; i < rowCount * colCount; i++) {
        if (tiles[i]->getIsMine() && tiles[i]->getRevealed()) {
            lost = true;
            return true;
        }
    }
    return false;
}

void Board::revealAllMines() {
    for (int i = 0; i < rowCount * colCount; i++) {
        if (tiles[i]->getIsMine()) {
            tiles[i]->reveal();
        }
    }
}


void Board::flagAllMines() {
    for (int i = 0; i < rowCount * colCount; i++) {
        if (tiles[i]->getIsMine() && tiles[i]->getIsFlagged() == false) {
            tiles[i]->toggleFlag();
        }
    }
}


int Board::getRemainingMines() {
    int flags = 0;
    for (int i = 0; i < rowCount * colCount; i++) {
        if (tiles[i]->getIsFlagged()) {
            flags++;
        }
    }
    int remainingMines = mineCount;
    remainingMines -= flags;
    return remainingMines;
}

void Board::toggleDebug() {
    if (!debug && !won && !lost) {
        debug = true;
        for (int i = 0; i < rowCount * colCount; i++) {
            tiles[i]->showMine(debug);
        }
    }
    else if (debug && !won && !lost) {
        debug = false;
        for (int i = 0; i < rowCount * colCount; i++) {
            tiles[i]->showMine(debug);
        }
    }
}

void Board::togglePause() {
    if (!pause && !won && !lost) {
        pause = true;
        for (int i = 0; i < rowCount * colCount; i++) {
            tiles[i]->toggleHidden();
        }
    }
    else if (pause && !won && !lost) {
        pause = false;
        for (int i = 0; i < rowCount * colCount; i++) {
            tiles[i]->toggleHidden();
        }
    }
}

void Board::draw() {
    for (int i = 0; i < rowCount * colCount; i++) {
        tiles[i]->draw(window);
    }
}

//
//Timer
//

Timer::Timer() {}

void Timer::start() {
    if (!running) {
        running = true;
        startTime = chrono::high_resolution_clock::now();
    }
}

void Timer::stop() {
    if (running) {
        storedTime += chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - startTime).count();
        running = false;
    }
}

void Timer::reset() {
    running = false;
    storedTime = 0;
}

int Timer::getTime() {
    if (running) {
        return storedTime + chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - startTime).count();
    }
    else {
        return storedTime;
    }
}


//
//Game Screen
//

GameScreen::GameScreen(int colCount, int rowCount, int mineCount, string playerName, sf::RenderWindow *window) {
    this->window = window;
    this->playerName = playerName;
    this->colCount = colCount;
    this->rowCount = rowCount;
    this->mineCount = mineCount;

    textureManager = new TextureManager();
    board = new Board(colCount, rowCount, mineCount, window);
    setUpButtons(colCount, rowCount);

    timer.start();

}

GameScreen::~GameScreen() {
    delete board;
    delete textureManager;
}


void GameScreen::setUpButtons(int colCount, int rowCount) {
    Button happy(textureManager, "files/images/face_happy.png", (((colCount / 2.0) * 32) - 32), (32 * (rowCount + 0.5)), "restart");
    buttons.push_back(happy);
    Button debug(textureManager, "files/images/debug.png", ((colCount * 32) - 304), (32 * (rowCount + 0.5)), "debug");
    buttons.push_back(debug);
    Button pause(textureManager, "files/images/pause.png", ((colCount * 32) - 240), (32 * (rowCount + 0.5)), "pause");
    buttons.push_back(pause);
    Button leaderboard(textureManager, "files/images/leaderboard.png", ((colCount * 32) - 176), (32 * (rowCount + 0.5)), "leaderboard");
    buttons.push_back(leaderboard);
}

void GameScreen::run() {
    while (window -> isOpen()) {
        while (const std::optional event = window->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window->close();
            }
            if (event.has_value()) {
                handleMouseEvents(*event);
            }
        }
        if (!gameOver){
        gameOver = checkConditions();
            }
        draw();
    }
}

void GameScreen::handleMouseEvents(const sf::Event& event) {
    if (const auto clicked = event.getIf<sf::Event::MouseButtonPressed>()) {
        int x = clicked->position.x;
        int y = clicked->position.y;
        for (int i = 0; i < buttons.size(); i++) {
            if (buttons[i].isClicked(x, y)) {
                return buttonPressed(buttons[i].getText());
            }
        }
        if (clicked->button == sf::Mouse::Button::Left && !gameOver) {
            board->LeftClick(x, y);
        }
        else if (clicked->button == sf::Mouse::Button::Right && !gameOver) {
            board->RightClick(x, y);
        }
    }
}

bool GameScreen::checkConditions() {
    if (board->checkWinCondition()) {
        timer.stop();
        board->flagAllMines();
        buttons[0].setTexture(textureManager, "files/images/face_win.png");
        draw();
        fixData();
        unsigned int x = (colCount * 16);
        unsigned int y = ((rowCount * 16) + 50);
        sf::RenderWindow leaderboardWindow(sf::VideoMode({x, y}), "Leaderboard");
        Leaderboard((colCount * 16), ((rowCount * 16) + 50), &leaderboardWindow);

        return true;
    }
    else if (board->checkLoseCondition()) {
        timer.stop();
        board->revealAllMines();
        buttons[0].setTexture(textureManager, "files/images/face_lose.png");
        return true;

    }
    return false;
}

void GameScreen::fixData() {
    map<string, string> leaderboardMap;
    ifstream leaderboardFile("files/leaderboard.txt");
    if (!leaderboardFile.is_open()) {
        cerr << "Could not open file" << endl;
        return;
    }
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
        if (name[name.size()-1] == '*') {
            name.pop_back();
        }
        leaderboardMap[time] = name;
    }
    leaderboardFile.close();
    int totalseconds = timer.getTime();
    int minutes = totalseconds / 60;
    int seconds = totalseconds % 60;
    string time;
    if (minutes >= 10 && seconds >= 10) {
        time = to_string(minutes) + ":" + to_string(seconds);
    }
    else if (minutes < 10 && seconds >= 10) {
        time = "0" + to_string(minutes) + ":" + to_string(seconds);
    }
    else if (minutes >= 10 && seconds < 10) {
        time = to_string(minutes) + ":0" + to_string(seconds);
    }
    else if (minutes < 10 && seconds < 10) {
        time = "0" + to_string(minutes) + ":0" + to_string(seconds);
    }
    leaderboardMap[time] = playerName + '*';
    ofstream leaderboardFileOut("files/leaderboard.txt");
    if (!leaderboardFileOut.is_open()) {
        cerr << "Could not open file" << endl;
    }
    int counter = 0;
    for (auto iter  = leaderboardMap.begin(); iter != leaderboardMap.end() && counter < 5; iter++) {
        leaderboardFileOut << iter->first << ", " << iter->second << endl;
        counter++;
    }
    leaderboardFileOut.close();
}

void GameScreen::buttonPressed(string buttonName) {
    if (buttonName == "restart" ) {
        board->reset();
        timer.reset();
        timer.start();
        gameOver = false;
        buttons[0].setTexture(textureManager, "files/images/face_happy.png");
        buttons[2].setTexture(textureManager, "files/images/pause.png");
    }
    else if (buttonName == "debug" ) {
        board->toggleDebug();
    }
    else if (buttonName == "pause" ) {
        if (!paused) {
            board->togglePause();
            buttons[2].setTexture(textureManager, "files/images/play.png");
            paused = true;
            timer.stop();
        }
        else if (paused) {
            board->togglePause();
            buttons[2].setTexture(textureManager, "files/images/pause.png");
            paused = false;
            timer.start();
        }
    }
    else if (buttonName == "leaderboard" ) {
        timer.stop();
        unsigned int x = (colCount * 16);
        unsigned int y = ((rowCount * 16) + 50);
        sf::RenderWindow leaderboardWindow(sf::VideoMode({x, y}), "Leaderboard");
        Leaderboard((colCount * 16), ((rowCount * 16) + 50), &leaderboardWindow);
        timer.start();
    }
}

void GameScreen::drawOneDigit(int number, float x, float y) {
    sf::Texture* digits = textureManager->getTexture("files/images/digits.png");
    sf::Sprite digit(*digits);
    digit.setTextureRect(sf::IntRect({number * 21, 0}, {21, 32}));
    digit.setPosition({x, y});
    window->draw(digit);
}

void GameScreen::drawMineCounter() {
    int remainingMines = board->getRemainingMines();
    if (remainingMines < 0) {
        drawOneDigit(10, 12, (32 * (rowCount + 0.5) + 16));
        remainingMines *= -1;
    }
    int hundreds = remainingMines / 100;
    drawOneDigit(hundreds, 33, (32 * (rowCount + 0.5) + 16));
    int tens = (remainingMines - (hundreds * 100)) / 10;
    drawOneDigit(tens, 54, (32 * (rowCount + 0.5) + 16));
    int ones = (remainingMines - (hundreds * 100) - (tens * 10));
    drawOneDigit(ones, 75, (32 * (rowCount + 0.5) + 16));

}

void GameScreen::drawTimer() {
    int timeSeconds = timer.getTime();
    int minutes = timeSeconds / 60;
    int seconds = timeSeconds % 60;
    int minute1, minute2, second1, second2;
    if (minutes < 10) {
        minute1 = 0;
        minute2 = minutes;
    }
    else if (minutes >= 10) {
        minute1 = minutes / 10;
        minute2 = minutes % 10;
    }
    if (seconds < 10) {
        second1 = 0;
        second2 = seconds;
    }
    else if (seconds >= 10) {
        second1 = seconds / 10;
        second2 = seconds % 10;
    }
    drawOneDigit(minute1, ((colCount * 32) - 97), (32 * (rowCount + 0.5) + 16));
    drawOneDigit(minute2, ((colCount * 32) - 76), (32 * (rowCount + 0.5) + 16));
    drawOneDigit(second1, ((colCount * 32) - 54), (32 * (rowCount + 0.5) + 16));
    drawOneDigit(second2, ((colCount * 32) - 33), (32 * (rowCount + 0.5) + 16));
}

void GameScreen::draw() {
    window->clear(sf::Color::White);
    board->draw();
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].draw(window);
    }
    drawMineCounter();
    drawTimer();
    window->display();
}