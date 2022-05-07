#pragma once

#include "Entity.h"
#include "constants.h"
#include <vector>
#include <memory>
#include "Map.h"


class Attacker: public Entity
{
public:
    Attacker(int posX, int posY, int maxHealth, Map* map);
//    ~Attacker() = default;
    char getSymbol() override;
    ~Attacker() = default;


    bool checkRadius() override;
    void setPosition(int x, int y);
    void place(int x, int y);


};
class basicAttacker final: public Attacker
{
public:
    basicAttacker(int posX, int posY, int maxHealth, Map* map);
    char getSymbol() override;
    bool checkRadius() override;
};

class FastAttacker final: public Attacker
{

};

class chargerAttacker final: public Attacker
{

};
