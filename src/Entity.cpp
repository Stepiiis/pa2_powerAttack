#pragma once
#include "Entity.h"
#include "constants.h"
#include "ncurses.h"


Entity::Entity(int posX, int posY, int maxHealth)
    :   m_x(posX), m_y(posY), m_hp(maxHealth),m_maxhp(maxHealth)
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

char Entity::getSymbol(){
    return m_symbol;
}

