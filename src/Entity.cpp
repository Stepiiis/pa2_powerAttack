
#include "Entity.h"

Entity::Entity(int posX, int posY, int hp, int dmg, char symbol, int radius, int attackSpeed, Map* map,int attackerID)
    :   m_x(posX), m_y(posY),  m_symbol(symbol), m_id(attackerID),  m_hp(hp), m_sharedMap(map), m_damage(dmg), m_attackSpeed(attackSpeed), m_radius(radius)
    {
        m_currentFocusID = -10;
    }

bool Entity::move(int x, int y)
    {
        m_x = x;
        m_y = y;
        return true;
    }

bool Entity::takeDamage(int damage){
        m_hp -= damage;
        if(m_hp<=0)
            alive = false;
        return alive;
    }

bool Entity::giveDamage(int damage, Entity* target){
        target->takeDamage(damage);
        return true;
    }

bool Entity::destroy() {
    m_sharedMap->revertCell(m_x, m_y);
    return true;
}

int Entity::getHP() const{ return m_hp; }

void Entity::draw(){
    m_sharedMap->setEntity( m_x, m_y, this);
    m_sharedMap->refreshWindow();
}

size_t Entity::getID()const {
    return m_id;
}

std::pair<int, int> Entity::getPosition() const {
    return {m_x,m_y};
}

char Entity::getSymbol() const {
    return m_symbol;
}




/*
 * calculates all offsets on a grid which are in reach of steps of a given radius
 *
 * redius 1 deltas
 *  x
 * xxx
 *  x
 * radius 2 deltas
 *   x
 *  xxx
 * xxxxx
 *  xxx
 *   x
 * radius 3 deltas
 *    x
 *   xxx
 *  xxxxx
 * xxxxxxx
 *  xxxxx
 *   xxx
 *    x
 */
void Entity::calculateDeltas() {
    std::vector<std::pair<int,int>> init_vec;
    for(int i = 0; i<=m_radius; ++i){
        init_vec.emplace_back(0,i);
    }
    for(const auto & point: init_vec ){
        int steps = point.second;
        auto temp = point;
        auto pointNE = point; // NE, NW, SE, SW are coordinates where the point will be moving
        auto pointNW = point;
        auto pointSE = point;
        auto pointSW = point;
        pointNE.second = -point.second;
        pointNW.second = -point.second;

        m_deltas.emplace(pointNE);
        m_deltas.emplace(pointSE);

        while(steps>0)
        {
            --steps;
            m_deltas.emplace(++pointSE.first, --pointSE.second);
            m_deltas.emplace(--pointSW.first, --pointSW.second);
            m_deltas.emplace(++pointNE.first, ++pointNE.second);
            m_deltas.emplace(--pointNW.first, ++pointNW.second);
        }
    }
}

// parameter specifies for what are we looking for
bool Entity::checkRadius(Point::PointType type){
    std::map<size_t, std::pair<int,int>> towers;
    std::map<std::pair<int,int>, size_t> closestTowers;
    bool foundFocus = false;
    for(const auto& [x,y] : m_deltas){
        if(m_x+x<0 || m_y+y<0 || m_x+x>=m_sharedMap->m_map[m_y].size()|| m_y+y>=m_sharedMap->m_map.size())
            continue;
        if(m_sharedMap->m_map[m_y+y][m_x+y]._type == type) {
            if(m_sharedMap->checkClearSight({m_x,m_y},{m_x+x,m_y+y})){
                if(hasFocus) {
                    if ((size_t) m_sharedMap->m_map[m_y + y][m_x + x]._ident == m_currentFocusID){
                        foundFocus = true;
                        towers.emplace((size_t)m_sharedMap->m_map[m_y+y][m_x+x]._ident, std::make_pair(x, y));
                        break;
                    }
                }
                closestTowers.emplace(std::make_pair(x,y), (size_t)m_sharedMap->m_map[m_y+y][m_x+x]._ident);
                towers.emplace((size_t)m_sharedMap->m_map[m_y+y][m_x+x]._ident, std::make_pair(x, y));
            }
        }
    }
    if(foundFocus)
        return true;
    else if(closestTowers.empty())
        return false;
    else {
        m_currentFocusID = closestTowers.begin()->second;
        hasFocus = true;
        return true;
    }
}

size_t Entity::getCurrentFocus() const {
    return m_currentFocusID;
}

int Entity::getDamage() const {
    return m_damage;
}
