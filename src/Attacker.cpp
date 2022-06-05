#include "Attacker.h"




Attacker::Attacker(int posX, int posY,int hp, int dmg, char symbol, int radius,int attackSpeed, Map * map,int attackerID)
: Entity(posX, posY, hp,dmg, symbol, radius, attackSpeed, map, attackerID)
{
}

basicAttacker::basicAttacker(int posX, int posY, defEntity& def, Map* map, int id, int hp)
:Attacker(posX, posY,def[BASICA]["hp"],def[BASICA]["dmg"],(char)def[BASICA]["symbol"],def[BASICA]["rng"],def[BASICA]["atkspeed"], map,id)
    {
        m_movementSpeed = def[BASICA]["mov"];
        m_defaultMovementSpeed = m_movementSpeed;
        if(hp!=-10)
            m_hp = hp;
        calculateDeltas();
    }
fastAttacker::fastAttacker(int posX, int posY, defEntity &def, Map *map, int id, int hp )
: Attacker(posX, posY,def[FASTA]["hp"],def[FASTA]["dmg"],(char)def[FASTA]["symbol"],def[FASTA]["rng"], def[FASTA]["atkspeed"],map,id)
{
    m_movementSpeed = def[FASTA]["mov"];
    m_defaultMovementSpeed = m_movementSpeed;
    if(hp!=-10)
        m_hp = hp;
    calculateDeltas();
}

chargerAttacker::chargerAttacker(int posX, int posY,defEntity& def, Map *map, int id, int hp)
: Attacker(posX, posY,def[CHARGERA]["hp"],def[CHARGERA]["dmg"],(char)def[CHARGERA]["symbol"],def[CHARGERA]["rng"],def[CHARGERA]["atkspeed"], map,id)
{
    m_movementSpeed = def[CHARGERA]["mov"];
    m_defaultMovementSpeed = m_movementSpeed;
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


bool Attacker::findShortestPath(Point::PointType endType) {
    m_path.clear();
//    using Type = Point::PointType;
     Point start = Point(m_x,m_y);
     Point target = Point(-10,-10); // dummy point used to determine if we found a target
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
            if(this->checkSpecialization(neighbor)){
                return;
            }
            if(neighbor == current || neighbor._type == Point::Wall || visited.count(neighbor) != 0 )
                return;
            visited.emplace(neighbor, current);
            q.push_back(neighbor);

            if(neighbor._type == endType || this->isTarget(neighbor)){
                target = neighbor;
                return;
            }
        });
        if(target._x != -10)
            break;
     }
     if(target._x == -10)
         return false;

     m_path.push_back(target);

     while(target != start){
         target = visited[target];
         m_path.push_front(target);
     }
     if(this->getTypeName()==CHARGERA && m_path.back()._type != Point::Exit)
         m_path.pop_back();
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

void Attacker::addEffects(const CEffects eff) {
    m_effects += eff;
}

CEffects Attacker::getEffects() const {
    return m_effects;
}



bool basicAttacker::isTarget(const Point &p) const{
    return false; // basicAttacker has a target type Point::Exit which is provided as a default argument in the function findShortestPath
}

bool Attacker::moveOnPath() {
    if(m_path.empty())
        return false;
    m_cycleCnt++;
    if((m_cycleCnt % m_movementSpeed) == 0)
    {
        m_cycleCnt = 0;
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

bool Attacker::hasSlowEffect() const {
    return m_effects.m_slowEffect != 0;
}

bool Attacker::setSlowerMovement() {
    m_movementSpeed = floor(m_defaultMovementSpeed * 1.5);
    m_effects.m_slowEffect--;
    return true;
}

bool Attacker::setNormalMovement() {
    m_movementSpeed = m_defaultMovementSpeed;
    return true;
}

std::deque<Point>& Attacker::getPath() {
    return m_path;
}

Point::PointType Attacker::pointToHide() {
    return Point::Entry;
}

std::string basicAttacker::getTypeName() const {
    return {"basicAttacker"};
}

bool basicAttacker::checkSpecialization(const Point &point) {
    if(point._type == Point::Tower || point._type == Point::Water)
        return true;
    return false;
}


bool fastAttacker::isTarget(const Point &p) const {
    return p._type == Point::Exit;
}

std::string fastAttacker::getTypeName() const {
    return {"fastAttacker"};
}

bool fastAttacker::checkSpecialization(const Point &point) {
    if(point._inRadiusOf.count(Point::Tower) == 1)
        return true;
    if(point._type == Point::Attacker || point._type == Point::Entry || point._type== Point::Tower)
        return true;
    return false;
}

Point::PointType fastAttacker::pointToHide() {
    return Point::Water;
}


// returns true if the point is a tower
bool chargerAttacker::isTarget(const Point &p) const {
    return p._type == Point::Tower || p._type == Point::Exit;
}

std::string chargerAttacker::getTypeName() const {
    return {"chargerAttacker"};
}

bool chargerAttacker::checkSpecialization(const Point &point) {
    return point._type == Point::Attacker || point._type == Point::Entry || point._type == Point::Water;
}
