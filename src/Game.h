#pragma once
#include "HelpStructs.h"
class Game {
    Game(int);
    ~Game();
    bool run();
    bool save();
    bool load();
    bool exit();
};
int mainMenu(terminal term);
