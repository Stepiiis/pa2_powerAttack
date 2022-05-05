#pragma once
#include "Entity.h"
#include "Attacker.h"
#include <memory>

class Tower final: public Entity {
public:
    Tower(int x, int y, int hp, std::shared_ptr<Map> map);
    char getSymbol() override;
    bool checkRadius() override;
private:
    int _x;
    int _y;
};

// shoots twice as fast as regular tower
class fastTower final: public Entity{
public:
    fastTower(int x, int y, int hp, std::shared_ptr<Map> map);
    char getSymbol() override;
    bool checkRadius() override;
};

class highDamageTower final: public Entity
        {

        };

class slowEffectTower final: public Entity
        {

        };