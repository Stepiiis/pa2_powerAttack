#pragma once
#include "HelpStructs.h"
#include <ncurses.h>
#include <vector>
#include <string>
class CGame {
public:
    CGame(int);      // int is the current level that we want to start on (1-5);
    ~CGame();        // closes the window and frees the memory
    bool start();
    bool save();
    bool load();
    bool exit();
};

struct CMenu{
    CMenu(std::initializer_list<std::string> text, std::initializer_list<std::string> options);
    std::initializer_list<std::string> _text;
    std::initializer_list<std::string> _options;
};

int newMenu(CMenu&& menu); // returns the number of option that was chosen

int mainMenu(terminal term);

int loadMenu(terminal term);

void printShrek(WINDOW* menu_win, int posY, int posX);
void clearShrek(WINDOW* menu_win, int posY, int posX);