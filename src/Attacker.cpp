#pragma once
#include "Attacker.h"
#include "Entity.h"
#include "Map.h"
#include "constants.h"

Attacker::Attacker(int posX, int posY, int maxHealth,Map *map)
: Entity(posX, posY, maxHealth, map)
{
}

basicAttacker::basicAttacker(int posX, int posY, int maxHealth,Map* map)
:Attacker(posX, posY, maxHealth,map)
    {
        m_symbol = '$';
        m_radius = 2; // the slower and more HP version will have range of 5 fire
    }

char inline Attacker::getSymbol()
{
    return m_symbol;

}char inline basicAttacker::getSymbol()
{
    return m_symbol;
}

//    bool Attacker::move (int x, int y)
//    {
//        if(x+y < 0 || m_x+x > MAP_HEIGHT || m_y+y > MAP_WIDTH)
//            return false;
//        m_x += x;
//        m_y += y;
//        return true;
//    }

// do not use
void Attacker::setPosition(int x, int y){
    m_sharedMap->m_map[x][y].m_symbol = m_symbol;
}



bool Attacker::checkRadius(){}

bool basicAttacker::checkRadius(){}