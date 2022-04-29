#pragma once

#include "Entity.h"
#include "constants.h"


class basicAttacker: public Entity
{
public:
    basicAttacker(int posX, int posY, int maxHealth);
    
    ~basicAttacker();
    virtual bool checkRadius();
    virtual void attack();
    virtual void update();

    bool move (int x, int y) override;

    int speed;
};