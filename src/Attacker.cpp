#include "Attacker.h"




Attacker::Attacker(int posX, int posY,int hp, int dmg, char symbol, int radius,int attackSpeed, Map * map,int attackerID)
: Entity(posX, posY, hp,dmg, symbol, radius, attackSpeed, map, attackerID)
{
    m_slowEffectDuration = 0;
}

basicAttacker::basicAttacker(int posX, int posY, defEntity& def, Map* map, int id, int hp)
:Attacker(posX, posY,def[BASICA]["hp"],def[BASICA]["dmg"],(char)def[BASICA]["symbol"],def[BASICA]["rng"],def[BASICA]["atkspeed"], map,id)
    {
        m_movementSpeed = def[BASICA]["mov"];
        if(hp!=-10)
            m_hp = hp;
        calculateDeltas();
    }
fastAttacker::fastAttacker(int posX, int posY, defEntity &def, Map *map, int id, int hp )
: Attacker(posX, posY,def[FASTA]["hp"],def[FASTA]["dmg"],(char)def[FASTA]["symbol"],def[FASTA]["rng"], def[FASTA]["atkspeed"],map,id)
{
    m_movementSpeed = def[FASTA]["mov"];
    if(hp!=-10)
        m_hp = hp;
    calculateDeltas();
}

chargerAttacker::chargerAttacker(int posX, int posY,defEntity& def, Map *map, int id, int hp)
: Attacker(posX, posY,def[CHARGERA]["hp"],def[CHARGERA]["dmg"],(char)def[CHARGERA]["symbol"],def[CHARGERA]["rng"],def[CHARGERA]["atkspeed"], map,id)
{
    m_movementSpeed = def[CHARGERA]["mov"];
    if(hp!=-10)
        m_hp = hp;
    calculateDeltas();
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
     Point current = start;
     q.push_back(start);
     while(!q.empty())
     {
        current = q.front();
        q.pop_front();

        m_sharedMap->forEachNeighbor(current , [&](const Point& neighbor)
        {
            if(neighbor == current || neighbor._type == Point::Entry || neighbor._type == Point::Wall || neighbor._type == Point::Tower || visited.count(neighbor) != 0 )
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
    if(m_path.empty())
        return {-10,-10};
    Point temp = m_path.front();
    return temp;
}

Point::PointType Attacker::getType() {
    return Point::Attacker;
}

void Attacker::setEffects(CEffects &eff) {
    m_slowEffectDuration=eff.m_slowEffect;
}


Attacker::~Attacker() = default;


bool basicAttacker::isTarget(const Point &p) const{
    return p._type == Point::Exit;
}

bool basicAttacker::moveOnPath() {
    if(m_path.empty())
        return false;
    cycleCnt++;
    if(cycleCnt % m_movementSpeed == 0)
    {
        cycleCnt = 0;
        Point next = m_path.front();
        if(next._type == Point::Exit)
            return false;
        if(next._type == Point::Attacker)
            return true;
        m_path.pop_front();
        if(!m_sharedMap->updateMap(next._x, next._y, this))
            return false;
        return true;
    }
    return true;
}



bool fastAttacker::isTarget(const Point &p) const {
    return p._type == Point::Exit;
}

bool fastAttacker::moveOnPath() {
    throw(notImplementedException("fastAttacker moveOnPath"));
}


// returns true if the point is a tower
bool chargerAttacker::isTarget(const Point &p) const {
    return p._type == Point::Tower || p._type == Point::Exit;
}

bool chargerAttacker::moveOnPath() {
    throw(notImplementedException("chargerAttacker moveOnPath"));
}
