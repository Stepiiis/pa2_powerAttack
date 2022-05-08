#pragma once
#include "Tower.h"
#include "exceptions.h"

Tower::Tower(int x, int y, int hp, Map* map)
: Entity(x,y, hp, map)
{}

basicTower::basicTower(int x, int y, int hp, Map* map)
        : Tower(x,y, hp, map)
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
}
char basicTower::getSymbol() {
    return m_symbol;
}
char fastTower::getSymbol(){
    return m_symbol;
}

bool Tower::checkRadius() { throw notImplementedException("tower checkRadius"); }
bool basicTower::checkRadius() { throw notImplementedException("basicTower checkRadius"); }
bool fastTower::checkRadius() { throw notImplementedException("fastTower checkRadius"); }
