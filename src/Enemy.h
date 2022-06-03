#pragma once
#include "Player.h"
#include "Tower.h"
#include "Map.h"
#include "Definitions.h"
#include "logFile.h"
#include <memory>
#include <vector>
#include <ncurses.h>
#include "Map.h"
#include "constants.h"
#include <random>

// handler of building and attacking of towers
class Enemy{
public:
    Enemy(Map* map,defEntity def, int dif);
    ~Enemy();
    void clearTowers(); // erases old towers from the map
    void findEmptySpaces(); // finds empty spaces where towers can be built
    void createTowers(); // creates towers according to the difficulty chosen
    void printTowers(); // prints all created towers onto the map
    size_t getTowerCount(); // returns the number of towers
    bool damageTowers(std::vector<std::pair<int, int> > &towers); // damages all towers in the vector
    bool createNewTower(int type, int x, int y, int hp, int id); // 0 = BASIC, 1 = FAST, 2 = STRONG, 3 = SLOWEF
protected:
    Map* _map;
    std::map<int,std::unique_ptr<Tower> > _towers;
    int _towerIDcnt;
    int _difficulty;
    defEntity _def; // definitions of towers loaded from file
    std::vector<std::vector<char> > _emptySpaces;
};