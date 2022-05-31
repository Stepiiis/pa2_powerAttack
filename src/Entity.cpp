
#include "Entity.h"

Entity::Entity(int posX, int posY, int maxHealth, Map* map,int attackerID)
    :   m_x(posX), m_y(posY), m_hp(maxHealth),m_maxhp(maxHealth), m_sharedMap(map), m_id(attackerID)
    {}

bool Entity::move(int x, int y)
    {
        m_x = x;
        m_y = y;
        return true;
    }

bool Entity::takeDamage(int damage){
        m_hp -= damage;
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
