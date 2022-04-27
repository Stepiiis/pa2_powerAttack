#pragma once
#include "Attacker.h"
#include "constants.h"

basicAttacker::basicAttacker(int posX, int posY, uint maxHealth)
: Entity(posX, posY, maxHealth)
    {
        m_symbol = '$';
    }

    basciAttacker::bool move (int x, int y) override;
    {
        if(x+y < 0 || m_x+x > MAP_HEIGHT || m_y+y > MAP_WIDTH)
            return;
        m_x += x;
        m_y += y;
    }