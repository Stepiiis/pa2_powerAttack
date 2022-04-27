#pragma once

#include "Entity.h"
#include "constants.h"
class basicAttacker: Entity
{
public:
    basicAttacker(int posX, int posY, uint maxHealth);
    
    ~Attacker();

    bool move (int x, int y) override;

    void speed;
};