#pragma once
#include "Tower.h"

Tower::Tower(int x, int y, int hp, Map* map)
: Entity(x,y, hp, map)
{
    m_symbol = 'I';
    m_radius = 4;
}

fastTower::fastTower(int x, int y, int hp, Map* map)
        : Tower(x,y, hp, map)
{
    m_symbol = '&';
    m_radius = 3;
}

char Tower::getSymbol(){
    return m_symbol;
}char fastTower::getSymbol(){
    return m_symbol;
}

bool Tower::checkRadius() {}
bool fastTower::checkRadius() {}
