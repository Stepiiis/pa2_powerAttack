#pragma once

#include <vector>
#include <string>
#include <set>
#include <ncurses.h>
#include <experimental/filesystem>
#include "HelpStructs.h"


struct CMenu{
    CMenu() = default;
    CMenu(std::vector<std::string> text, std::vector<std::string> options);
    int show(bool wait = false); // returns the number of option that was chosen, if wait is true, it will wait for input before showing menu
    void setMenu(std::vector<std::string> text, std::vector<std::string> options); // sets the menu

    static int mainMenu();

    static std::string saveMenu();

    static int loadMenu(std::vector<std::string>& save_names);

    static int optionsMenu();

    static void printShrek(WINDOW* menu_win, int posY, int posX);
    static void clearShrek(WINDOW* menu_win, int posY, int posX);

private:
    std::vector<std::string> _text;
    std::vector<std::string> _options;
};

