#pragma once
#include "Attacker.h"
#include "Entity.h"
#include "constants.h"

basicAttacker::basicAttacker(int posX, int posY, int maxHealth)
: Entity(posX, posY, maxHealth)
    {
        m_symbol = '$';
    }

    bool basicAttacker::move (int x, int y)
    {
        if(x+y < 0 || m_x+x > MAP_HEIGHT || m_y+y > MAP_WIDTH)
            return false;
        m_x += x;
        m_y += y;
        return true;
    }