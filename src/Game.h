#pragma once
#include "HelpStructs.h"
#include <ncurses.h>
#include "Attacker.h"
#include "Map.h"
#include "exceptions.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <string>
#include <ncurses.h>
#include <iostream>
#include <memory>
#include <sys/ioctl.h>
#include <experimental/filesystem>
#include "Definitions.h"
#include "Menu.h"

class CGame {
public:
    CGame() = default;
    ~CGame();           // closes the window and frees the memory
    bool init(int,int, const char * pathToSave = "");// first intiger specifies if we want to start a new game or load a saved game
                                                            // second int is difficulty level (1-3)
                                                            // third string is the name of save to load. if left empty, new game initiates with the difficulty level chosen
    bool initializeWindow();   // initializes the game window
    [[nodiscard]] bool start();       // starts the game from current level or from the saved game
    bool save();        // saves the game into /data/saves/save_*save_number*.txt specified in menu
    bool load(int = 0);     // loads map from /data/saves/save_*save_number*.txt
    bool loadFromSave(const std::string& path); // loads save from /data/saves/*save_name*
    bool exit();        // exits the game and asks the user if they want to save the current progress
    void draw();        // draws the current state of map
    void drawTowers();  // draws the towers according to difficulty
    void pause();       // pauses the game
    bool resume();      // resumes the game. checks if player inputed a char.
                        // if not, it calculates the path for every attacker and checks if there is a tower in their radius
                        // if there is, it attacks the tower. Then Towers check their radius and attack one of the players
                        //
                        // if the player inputs a char it creates a new attacker according to input
                        // if the player doesnt have enough money for the certain attacker, prints NOT ENOUGH MONEY to the bottom
                        // then runs the procedure again
    bool play();
    [[nodiscard]] bool pauseMenu(const char* msg = "");
    bool gameEnd(const char * );
protected:
    void highlightAttacker(int type);
    void drawAttackerDefs();
    Map _gameMap = Map();
    Player * _player = nullptr;
    Enemy * _tower_manager = nullptr;
    std::string _save_name;
    WINDOW * _game_window = nullptr;
    CDefinitions _definitions;
    int towers_destroyed;
    int _score;
    int _difficulty;

    void redrawTowers();

    void drawCurrentMoney();
};
