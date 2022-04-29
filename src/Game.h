#pragma once
#include "HelpStructs.h"
#include <ncurses.h>
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <string>
class CGame {
public:
    CGame(int,int, std::string = "");       // first int is the current level that we want to start on
                                            //                  player can choose from 1-5
                                            //                  0 means load game
                                            // second int is difficulty level (1-3)
                                            // third string is the name of save to load. if left empty, new game initiates with the difficulty level chosen
    ~CGame();           // closes the window and frees the memory
    bool start();       // starts the game from current level or from the saved game
    bool save();        // saves the game int /data/saves
    bool load(int = 0);     // loads map from /data/saves/*level_number*
    bool loadFromSave(std::string path); // loads save from /data/saves/*save_name*
    bool exit();        // exits the game
    void draw();        // draws the game
    void drawTowers(int);  // draws the towers according to difficulty
private:
    Map _gameMap;
    Player _player;
    Enemy _tower_manager;
    std::string _save_name;
    int _level;
    int _difficulty;
};

struct CMenu{
    CMenu(std::vector<std::string> text, std::vector<std::string> options);
    std::vector<std::string> _text;
    std::vector<std::string> _options;
};

int newMenu(const CMenu& menu); // returns the number of option that was chosen

int mainMenu();

int loadMenu(std::vector<std::string>& save_names);

void printShrek(WINDOW* menu_win, int posY, int posX);
void clearShrek(WINDOW* menu_win, int posY, int posX);