#pragma once
#include "Entity.h"
#include "Attacker.h"
#include <memory>

class Tower : public Entity {
public:
    Tower(int x, int y, int hp, Map* map,int towerID);
    char getSymbol() override;
    bool checkRadius() override;
};

// základní věž se symbolem T
class basicTower final: public Tower{
public:
    basicTower(int x, int y, int hp, Map* map,int towerID);
    char getSymbol() override;
    bool checkRadius() override;
};


// Střílí dvě rány za jednu interakci
// Zničení odmění hráče základní odměnou
class fastTower final: public Tower{
public:
    fastTower(int x, int y, int hp, Map* map,int towerID);
    char getSymbol() override;
    bool checkRadius() override;
};

// útok od této věže je ~1,5x větší než základ. je však 2x dražší
// odměna za zničení je 1,5x větší než fast tower
class highDamageTower final: public Tower
    {
public:
        highDamageTower(int x, int y, int hp, Map* map,int towerID);
        char getSymbol() override;
        bool checkRadius() override;
    };


// útok od této věže způsobí to, že attacker po dobu 4 cyklů nebude moct útočit na věže.
class slowEffectTower final: public Tower
        {
public:
    slowEffectTower(int x, int y, int hp, Map* map,int towerID);
    char getSymbol() override;
    bool checkRadius() override;
        };