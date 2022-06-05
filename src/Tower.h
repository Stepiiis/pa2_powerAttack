#pragma once
#include "Entity.h"
#include "Attacker.h"
#include "exceptions.h"
#include <memory>


class Tower : public Entity {
public:
    Tower(int posX, int posY,int hp, int dmg, char symbol, int radius, int attackSpeed, Map * map,int attackerID);
    ~Tower() override = default;
    bool operator < (Tower & rhs);
    bool operator > (Tower & rhs);
    virtual CEffects  getEffect() const =0; // returns effects which should be given on attack to enemy
    Point::PointType getType() override;
    [[nodiscard]] std::string getTypeName() const override = 0;
};


// základní věž se symbolem T
class basicTower final: public Tower{
public:
    basicTower(int x, int y, defEntity &def, Map* map,int towerID, int hp = -10);
    [[nodiscard]] std::string getTypeName() const override;
    CEffects  getEffect() const override;
};


// Střílí dvě rány za jednu interakci
// Zničení odmění hráče základní odměnou
class fastTower final: public Tower{
public:
    fastTower(int x, int y, defEntity& def, Map* map,int towerID, int hp = -10);
    [[nodiscard]] std::string getTypeName() const override;
    CEffects  getEffect() const override;
};

// útok od této věže je ~1,5x větší než základ. je však 2x dražší
// odměna za zničení je 1,5x větší než fast tower
class highDamageTower final: public Tower
    {
public:
    highDamageTower(int x, int y, defEntity &def, Map* map,int towerID, int hp = -10);
    [[nodiscard]] std::string getTypeName() const override;
    CEffects  getEffect() const override;
    };


// útok od této věže způsobí to, že attacker po dobu 4 cyklů nebude moct útočit na věže.
class slowEffectTower final: public Tower
        {
public:
    slowEffectTower(int x, int y, defEntity& def, Map* map,int , int hp = -10);
    [[nodiscard]] std::string getTypeName() const override;
    CEffects  getEffect() const override;
        };