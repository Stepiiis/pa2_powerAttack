#pragma once
#include "Attacker.h"
#include "Entity.h"
#include "Map.h"
#include "constants.h"
#include "exceptions.h"
#include "Definitions.h"

Attacker::Attacker(int posX, int posY, int maxHealth,Map *map, int id)
: Entity(posX, posY, maxHealth, map, id)
{
}

basicAttacker::basicAttacker(int posX, int posY, int maxHealth,Map* map, int id)
:Attacker(posX, posY, maxHealth,map,id)
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

//    bool Attacker::move (int _x, int y)
//    {
//        if(_x+y < 0 || m_x+_x > MAP_HEIGHT || m_y+y > MAP_WIDTH)
//            return false;
//        m_x += _x;
//        m_y += y;
//        return true;
//    }

// do not use
void Attacker::setPosition(int x, int y){
    m_sharedMap->m_map[x][y] = Point(x,y,m_symbol);
}



bool Attacker::checkRadius(){
    throw (notImplementedException("Attacker checkRadius"));
}

bool Attacker::operator<(Attacker &rhs) {
    return this->m_id<rhs.m_id;
}

bool Attacker::operator>(Attacker &rhs) {
    return this->m_id>rhs.m_id;
}


bool Attacker::findShortestPath(){
    m_path.clear();
//    using Type = Point::PointType;
     Point start = Point(m_x,m_y, '<');
     Point target = Point(-10,-10, '>'); // dummy point used to determine if we found a target
     std::map<Point, Point> visited;
     std::deque<Point> q;
     Point current;
     q.push_back(start);
     while(!q.empty())
     {
        current = q.front();
        q.pop_front();

        m_sharedMap->forEachNeighbor(current , [&](const Point& neighbor)
        {
            if(neighbor == current || neighbor._type == Point::Entry || neighbor._type == Point::Attacker || neighbor._type == Point::Wall || neighbor._type == Point::Tower || visited.count(neighbor) != 0 )
                return;
            visited.emplace(neighbor, current);
            q.push_back(neighbor);

            if(this->isTarget(neighbor)){
                target = neighbor;
                return;
            }
        });
     }

     if(target._x == -10)
         return false;
     m_path.push_back(target);
     while(target != start){
         target = visited[target];
         m_path.push_front(target);
     }
     m_path.pop_front();
     return true;
}

void Attacker::popPath() {
    if(!m_path.empty())
        m_path.pop_front();
}

Point Attacker::getNextPoint() {
    popPath();
    if(m_path.empty())
        return {-10,-10};
    Point& temp = m_path.front();
    return temp;
}

Point::PointType Attacker::getType() {
    return Point::Attacker;
}

Attacker::~Attacker() = default;

bool basicAttacker::checkRadius(){
    throw (notImplementedException("basicAttacker checkRadius"));
}

bool basicAttacker::isTarget(const Point &p) const{
    return p._type == Point::Exit;
}

bool basicAttacker::moveOnPath() {
    if(m_path.empty())
        return false;
    cycleCnt++;
//    if(cycleCnt % CDefinitions::_attackerDefinitions.at(BASICA).at("mov")== 0)
//    {
        Point next = m_path.front();
        m_path.pop_front();
        if(next._type == Point::Exit)
            return false;
        if(!m_sharedMap->updateMap(next._x, next._y, this))
            return false;
        return true;
//    }
}

fastAttacker::fastAttacker(int posX, int posY, int maxHealth, Map *map, int id) : Attacker(posX, posY, maxHealth, map, id) {

}

char fastAttacker::getSymbol() {
    throw(notImplementedException("fastAttacker getSymbol"));
}

bool fastAttacker::checkRadius() {
    throw(notImplementedException("fastAttacker checkRadius"));
}

bool fastAttacker::isTarget(const Point &p) const {
    return p._type == Point::Exit;
}

bool fastAttacker::moveOnPath() {
    throw(notImplementedException("fastAttacker moveOnPath"));
}

chargerAttacker::chargerAttacker(int posX, int posY, int maxHealth, Map *map, int id) : Attacker(posX, posY, maxHealth, map, id) {

}

char chargerAttacker::getSymbol() {
    throw(notImplementedException("chargerAttacker getSymbol"));
}

bool chargerAttacker::checkRadius() {
    throw(notImplementedException("chragerAttacker checkRadius"));

}

// returns true if the point is a tower
bool chargerAttacker::isTarget(const Point &p) const {
    return p._type == Point::Tower || p._type == Point::Exit;
}

bool chargerAttacker::moveOnPath() {
    throw(notImplementedException("chargerAttacker moveOnPath"));
}
