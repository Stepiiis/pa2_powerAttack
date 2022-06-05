#pragma once
#include "HelpStructs.h"
#include "Attacker.h"
#include "Map.h"
#include "exceptions.h"
#include "Player.h"
#include "Enemy.h"
#include "Definitions.h"
#include "Menu.h"
#include <vector>
#include <string>
#include <cstring>
#include <ncurses.h>
#include <iostream>
#include <memory>
#include <sys/ioctl.h>
#include <experimental/filesystem>
#include <ctime>

class CGame {
public:
    CGame() = default;
    ~CGame();           // closes the window and frees the memory
    bool play();        // starts the game
    void testRoutine(); // tests files to see if the game works correctly
protected:
    // first intiger specifies if we want to start a new game or load a saved game
    // second int is difficulty level (1-3)
    // third string is the name of save to load. if left empty, new game initiates with the difficulty level chosen
    bool init(int, const char * pathToSave = "");

    // initializes the game window
    bool initializeWindow();

    // starts the game from current level or from the saved game
    [[nodiscard]] bool start();
    // saves the game into /data/saves/save_*save_number*.txt specified in menu
    bool save( std::string saveName = "");
    // loads map from /assets/maps_*map_number*.txt
    bool load(int = 0);
    // loads save from /data/saves/*save_name*
    bool loadFromSave(const std::string& path);
    // exits the game and asks the user if they want to save the current progress
    bool exit();
    // draws the current state of map
    void draw();
    // draws the towers according to difficulty
    void drawTowers();
    // pauses the game
    void pause();
    // resumes the game. checks if player inputed a char.
    // if not, it calculates the path for every attacker and checks if there is a tower in their radius
    // if there is, it attacks the tower. Then Towers check their radius and attack one of the players
    // if the player inputs a char it creates a new attacker according to input
    // if the player doesnt have enough money for the certain attacker, prints NOT ENOUGH MONEY to the bottom
    // then runs the procedure again
    bool resume();
    // initializes the game and starts it
    [[nodiscard]] bool pauseMenu(const char* msg = "");
    bool gameEnd(const char * );
    void highlightAttacker(int type);
    void drawAttackerDefs();
    bool performAttacks();
    void setDifficulty(int);
    void setTowersDestroyed(int);

    Map m_gameMap = Map();
    Player * m_player = nullptr;
    Enemy * m_tower_manager = nullptr;
    std::string m_save_name;
    WINDOW * m_game_window = nullptr;
    CDefinitions m_definitions;
    int m_towers_destroyed{};
    int m_score{};
    int m_difficulty = 0;
    int errorTextLenght = 18;
    void redrawTowers();

    void drawCurrentMoney();

    void drawErrorMessage(const std::string & msg);

    void cleanErrorMessage(int i);

    void showError(const std::vector<std::string> &strings);
};
