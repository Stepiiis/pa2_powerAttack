
#include "Tower.h"


Tower::Tower(int posX, int posY,int hp, int dmg, char symbol, int radius, int attackSpeed,Map * map,int towerID)
: Entity( posX, posY, hp, dmg, symbol, radius, attackSpeed, map, towerID)
{
}

basicTower::basicTower(int x, int y, defEntity &def, Map* map,int towerID, int hp)
        : Tower( x, y, def[BASICT]["hp"], def[BASICT]["dmg"], (char)def[BASICT]["symbol"], def[BASICT]["rng"], def[BASICT]["atkspeed"], map, towerID)
{
    if(hp != -10)
        m_hp = hp;
    calculateDeltas();
}

fastTower::fastTower(int x, int y, defEntity &def, Map* map,int towerID, int hp)
        : Tower( x, y, def[FASTT]["hp"], def[FASTT]["dmg"], (char)def[FASTT]["symbol"], def[FASTT]["rng"], def[FASTT]["atkspeed"], map, towerID)
{
    if(hp != -10)
        m_hp = hp;
    calculateDeltas();
}


highDamageTower::highDamageTower(int x, int y, defEntity &def, Map *map, int towerID, int hp)
        : Tower( x, y, def[STRONGT]["hp"], def[STRONGT]["dmg"], (char)def[STRONGT]["symbol"], def[STRONGT]["rng"], def[STRONGT]["atkspeed"], map, towerID)
{
    if(hp != -10)
        m_hp = hp;
    calculateDeltas();
}


slowEffectTower::slowEffectTower(int x, int y, defEntity& def, Map *map, int towerID, int hp)
: Tower( x, y, def[SLOWET]["hp"], def[SLOWET]["dmg"], (char)def[SLOWET]["symbol"], def[SLOWET]["rng"], def[SLOWET]["atkspeed"], map, towerID)
{
    if(hp != -10)
        m_hp = hp;
    calculateDeltas();
}


Point::PointType Tower::getType() {
    return Point::Tower;
}

bool Tower::operator<(Tower &rhs) {
    return this->m_id<rhs.m_id;
}

bool Tower::operator>(Tower &rhs) {
    return this->m_id>rhs.m_id;
}
