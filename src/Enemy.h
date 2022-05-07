#pragma once
#include "Player.h"
#include "Tower.h"
#include "Map.h"
#include <memory>
#include <vector>
#include <ncurses.h>

// handler of building and attacking of towers
class Enemy : public Player{
public:
    Enemy(Map* map,int towerHp, int range, int damage, int dif);
    ~Enemy();
    void clearTowers(); // erases old towers from the map
    void findEmptySpaces(); // finds empty spaces where towers can be built
    void createTowers(); // creates towers according to the difficulty chosen
    void printTowers(); // prints all created towers onto the map
    std::vector<std::unique_ptr<Entity> > _towers;
protected:
    int _difficulty;
    std::vector<std::vector<char> > _emptySpaces;
};