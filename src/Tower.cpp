
#include "Tower.h"


Tower::Tower(int x, int y, defEntity def, Map* map, int towerID)
: Entity(x,y, std::move(def), map, towerID)
{
}

basicTower::basicTower(int x, int y, defEntity def, Map* map,int towerID)
        : Tower(x,y, std::move(def), map, towerID)
{
    m_symbol = (char)m_def[BASICT]["symbol"];
    m_radius = m_def[BASICT]["rng"];
    m_hp = m_def[BASICT]["hp"];
    m_damage = m_def[BASICT]["dmg"];
    calculateDeltas();
}

fastTower::fastTower(int x, int y, defEntity def, Map* map,int towerID)
        : Tower(x,y, std::move(def), map,towerID)
{
    m_symbol = (char)m_def[FASTT]["symbol"];
    m_radius = m_def[FASTT]["rng"];
    m_hp = m_def[FASTT]["hp"];
    m_damage = m_def[FASTT]["dmg"];
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

highDamageTower::highDamageTower(int x, int y, defEntity def, Map *map, int towerID) : Tower(x, y, std::move(def), map, towerID) {
    m_symbol = (char)m_def[STRONGT]["symbol"];
    m_radius = m_def[STRONGT]["rng"];
    m_hp = m_def[STRONGT]["hp"];
    m_damage = m_def[STRONGT]["dmg"];
    calculateDeltas();
}

slowEffectTower::slowEffectTower(int x, int y, defEntity def, Map *map, int towerID) : Tower(x, y, std::move(def), map, towerID) {
    m_symbol = (char)m_def[SLOWET]["symbol"];
    m_radius = m_def[SLOWET]["rng"];
    m_hp = m_def[SLOWET]["hp"];
    m_damage = m_def[SLOWET]["dmg"];
    calculateDeltas();
}