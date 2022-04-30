#pragma once
#include "Player.h"
#include "Tower.h"
#include "Map.h"
#include <memory>
#include <vector>
#include <ncurses.h>
class Enemy : public Player{
public:
    Enemy(Map& map,int towerHp, int range, int damage, int dif, int level);
    ~Enemy() = default;
    void clearTowers();
    void findEmptySpaces();
    void createTowers();
    void printTowers();
    std::vector<std::unique_ptr<Entity> > _towers;
protected:
    int _level;
    int _difficulty;
    std::vector<std::vector<char> > _emptySpaces;
};