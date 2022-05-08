#pragma once
#include "Player.h"
#include "Tower.h"
#include "Map.h"
#include "Definitions.h"
#include <memory>
#include <vector>
#include <ncurses.h>

// handler of building and attacking of towers
class Enemy{
public:
    Enemy(Map* map,towerDefs def, int dif);
    ~Enemy();
    void clearTowers(); // erases old towers from the map
    void findEmptySpaces(); // finds empty spaces where towers can be built
    void createTowers(); // creates towers according to the difficulty chosen
    void printTowers(); // prints all created towers onto the map
    int getTowerCount(); // returns the number of towers
protected:
    Map* _map;
    std::vector<std::unique_ptr<Tower> > _towers;
    int _difficulty;
    towerDefs _def; // definitions of towers loaded from file
    std::vector<std::vector<char> > _emptySpaces;
};