#pragma once
#include "Entity.h"
#include "constants.h"
#include "ncurses.h"


Entity::Entity(int posX, int posY, int maxHealth, std::shared_ptr<Map> map)
    :   m_x(posX), m_y(posY), m_hp(maxHealth),m_maxhp(maxHealth), _map(map)
    {}

bool Entity::move(int x, int y)
    {
        m_x += x;
        m_y += y;
        return true;
    }

void Entity::takeDamage(int damage){
        m_hp -= damage;
    }

bool Entity::giveDamage(int damage, Entity* target){
        target->takeDamage(damage);
        return true;
    }

bool Entity::destroy() {
    _map->m_map[m_x][m_y].type = Point::Empty;
    _map->m_map[m_x][m_y].m_symbol = ' ';

    return true;
}

int Entity::getHP() const{ return m_hp; }

void Entity::draw(){
    mvprintw(m_y, m_x, "%c", m_symbol);
}