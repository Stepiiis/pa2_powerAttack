#pragma once

#include "Entity.h"
#include "constants.h"


class basicAttacker: Entity
{
public:
    basicAttacker(int posX, int posY, int maxHealth);
    
    ~basicAttacker();

    bool move (int x, int y) override;

    int speed;
};