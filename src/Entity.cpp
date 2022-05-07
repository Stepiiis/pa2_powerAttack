#pragma once
#include "Entity.h"
#include "constants.h"
#include "ncurses.h"


Entity::Entity(int posX, int posY, int maxHealth, Map* map)
    :   m_x(posX), m_y(posY), m_hp(maxHealth),m_maxhp(maxHealth), m_sharedMap(map)
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
    m_sharedMap->m_map[m_x][m_y].type = Point::Empty;
    m_sharedMap->m_map[m_x][m_y].m_symbol = ' ';

    return true;
}

int Entity::getHP() const{ return m_hp; }

void Entity::draw(){
    m_sharedMap->updateEntity( m_x, m_y, this);
    m_sharedMap->refreshWindow();
}
