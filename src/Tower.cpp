
#include "Tower.h"


Tower::Tower(int x, int y, int hp, Map* map, int towerID)
: Entity(x,y, hp, map, towerID)
{}

basicTower::basicTower(int x, int y, int hp, Map* map,int towerID)
        : Tower(x,y, hp, map, towerID)
{
    m_symbol = 'I';
    m_radius = 4;
}

fastTower::fastTower(int x, int y, int hp, Map* map,int towerID)
        : Tower(x,y, hp, map,towerID)
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

Point::PointType Tower::getType() {
    return Point::Tower;
}

bool basicTower::checkRadius() { throw notImplementedException("basicTower checkRadius"); }
bool fastTower::checkRadius() { throw notImplementedException("fastTower checkRadius"); }
