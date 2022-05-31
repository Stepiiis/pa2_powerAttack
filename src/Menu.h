#pragma once

#include <vector>
#include <string>
#include <ncurses.h>
#include <experimental/filesystem>
#include "HelpStructs.h"

struct CMenu{
    CMenu(std::vector<std::string> text, std::vector<std::string> options);
    std::vector<std::string> _text;
    std::vector<std::string> _options;
};

int newMenu(const CMenu& menu, bool wait = false); // returns the number of option that was chosen

int mainMenu();

int loadMenu(std::vector<std::string>& save_names);

void printShrek(WINDOW* menu_win, int posY, int posX);
void clearShrek(WINDOW* menu_win, int posY, int posX);