#pragma once
#include "Player.h"
#include "Tower.h"
#include "Map.h"
#include <memory>
#include <vector>
class Enemy : public Player{
public:
    Enemy(Map& map,int towerHp, int range, int damage, int dif, int level);
    ~Enemy() = default;
    void clearTowers();
    void findEmptySpaces(Map&);
    void createTowers();
    void printTowers();
protected:
    int _level;
    int _difficulty;
    std::vector<std::unique_ptr<Entity> > _towers;
    std::vector<std::vector<char> > _emptySpaces;
};