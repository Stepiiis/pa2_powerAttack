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
    /**
    * @brief Initializes the game, next should be called start()
    * @param level specifies if we want to start a new game or load a saved game
    * @param pathToSavethe name of save to load. if left empty, new game initiates with the difficulty level chosen, otherwise the game loads the save with the name specified
    */
     bool init(int, const char * pathToSave = "");

    // initializes the game window
    bool initializeWindow();

    // starts the game from current level or from the saved game, resumes the game from current state
    [[nodiscard]] bool start();

    // saves the game into /data/saves/*saveName*.txt specified in menu
    bool save( std::string saveName = "");

    // loads map from /assets/maps_*map_number*.txt
    bool load(int = 0);

    // loads save from /data/saves/*save_name*
    bool loadFromSave(const std::string& path);

    // draws the towers according to difficulty
    void drawTowers();

    // resumes the game. checks if player inputed a char and acts accordingly.
    // if not, it calculates the path for every attacker and checks if there is a tower in their radius
    // if there is, it attacks the tower. Then Towers check their radius and attack one of the players
    // if the player inputs a char it creates a new attacker according to input
    // if the player doesnt have enough money for the certain attacker, prints NOT ENOUGH MONEY right of the map
    // runs procedure in loop
    bool resume();

    // initializes the game and starts it
    [[nodiscard]] bool pauseMenu(const char* msg = "");

    // draws new menu chich tells player if he won or lost and his score
    bool gameEnd(const char * );

    // highlights selected option
    void highlightAttacker(int type);

    // draws attacker definitions on map;
    void drawAttackerDefs();

    // permforms all attacks from atackers and towers
    bool performAttacks();

    void setDifficulty(int);
    void setTowersDestroyed(int);

    Map m_gameMap = Map(); // shared map
    Player * m_player = nullptr; // manages attackers
    Enemy * m_tower_manager = nullptr; // manages the towers
    std::string m_save_name;
    WINDOW * m_game_window = nullptr; // window pointer for ncurses
    CDefinitions m_definitions; // stroes definitions of all entities
    int m_towers_destroyed{};
    int m_score{};
    int m_difficulty = 0;
    int errorTextLenght = 18;

    void drawCurrentMoney();

    void drawErrorMessage(const std::string & msg);

    void cleanErrorMessage(int i);

    static void showError(const std::vector<std::string> &strings);
};
