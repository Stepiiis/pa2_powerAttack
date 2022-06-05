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

    /** @brief Closes the window and frees most of the memory used by ncurses
     * (some is left because ncurses allocates huge amounts on heap as buffer for screen
     * and relies on freeing by the system according to docs: https://invisible-island.net/ncurses/ncurses.faq.html#config_leaks,
     * which can be seen by running valgrind on the game executable)*/
    ~CGame();

    /** @brief starts the game. @returns true if the game was exited after a game instance and true if it was exited from main menu.*/
    bool play();

    /** @brief test routine used to test savefiles and instantiation of game. */
    void testRoutine();

protected:
    /**
    * @brief Initializes the game, next should be called start()
    * @param level specifies if we want to start a new game or loadMap a saved game
    * @param pathToSavethe name of save to loadMap. if left empty, new game initiates with the difficulty level chosen, otherwise the game loads the save with the name specified
    */
    bool init(int, const char *pathToSave = "");

    /** initializes the game window used.*/
    void initializeWindow();

    /// @brief starts the game from current level or from the saved game, resumes the game from current state.
    /// @returns true if game was won and false if game was lost.
    [[nodiscard]] bool start();

    /// @brief saves the game into /data/saves/*saveName*.txt specified in menu
    /// @returns true if save went correctly.
    bool save(std::string saveName = "");

    /// @brief Loads map from /assets/maps_*map_number*.txt
    /// @returns true if loaded map correctly
    /// @param level - specifies which map to load
    bool loadMap(int level = 0);

    /// @brief Loads save from /data/saves/*save_name*
    bool loadFromSave(const std::string &path);

    /// creates and draws the towers according to difficulty set in options menu.
    void drawTowers();

    /// resumes the game. checks if player inputed a char and acts accordingly.\n
    /// if not, it calculates the path for every attacker and checks if there is a tower in their radius\n
    /// if there is, it attacks the tower. Then Towers check their radius and attack one of the players\n
    /// if the player inputs a char it creates a new attacker according to input\n
    /// if the player doesnt have enough money for the certain attacker, prints NOT ENOUGH MONEY right of the map\n
    /// runs procedure in loop
    /// @returns true if game was won and false if game was exited (in pausemenu or by ESC button) or lost.
    bool resume();

    /// brings up pause menu in which player can save current state of game
    /// @returns what was chosen in the menu (false means exit, true means continue playing)
    [[nodiscard]] bool pauseMenu(const char *msg = "");

    /// draws new menu which tells player if he won or lost the game and his score
    /// @returns true if player chose to go back to main menu and false if player wants to outright exit the game.
    bool gameEnd(const char *);

    /// highlights selected option of attacker on screen
    /// @param type specifies which attacker to highlight (0 - basic, 1 - fast, 2 - charger)
    void highlightAttacker(int type);

    /// draws attacker definitions on map;
    void drawAttackerDefs();

    /// permforms all attacks from first atackers and then towers
    bool performAttacks();

    /// m_difficulty setter (0 - 2)
    /// @param def specifies max difficulty
    void setDifficulty(int def);

    /// m_difficulty getter
    /// @returns difficulty (0 - 2)
    int getDifficulty();

    /// @returns number of towers destroyed during game
    void setTowersDestroyed(int);

    /// Print current amount of money on screen.
    void drawCurrentMoney();

    /// Shows error popup screen if an error occured and waits for user input to go back
    /// @param strings vector of strings to be printed onto the error screen. length checks are not done. has to be done by caller.
    static void showError(const std::vector<std::string> &strings);


    /// Prints error message under current amount of money
    /// @param msg message to be printed on screen
    void drawErrorMessage(const std::string &msg);

    /// cleans error message from screen
    /// @param i length of previous message which was printed on screen so that it can be cleared
    void cleanErrorMessage(int i);


    Map m_gameMap = Map(); // shared map
    Player *m_player = nullptr; // manages attackers
    Enemy *m_tower_manager = nullptr; // manages the towers
    std::string m_save_name;
    WINDOW *m_game_window = nullptr; // window pointer for ncurses
    CDefinitions m_definitions; // stroes definitions of all entities
    int m_towers_destroyed{}; // number of towers destroyed during the game
    int m_score{};
    int m_difficulty = 0; // difficulty of game (0 - 2)
    int errorTextLenght = 18; // error text length printed on the right of screen
};
