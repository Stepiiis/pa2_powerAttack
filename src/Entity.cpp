#pragma once
#include "Entity.h"
#include "constants.h"


Entity::Entity(int posX, int posY, int maxHealth)
    :   m_x(posX), m_y(posY), m_maxhp(maxHealth), m_hp(maxHealth)
    {}

bool Entity::move(int x, int y)
    {
        m_x += x;
        m_y += y;
    }
void Entity::takeDamage(int damage){
        m_hp -= damage;
    }

bool Entity::giveDamage(int damage, Entity* target){
        target->takeDamage(damage);
    }

char Entity::getSymbol(){
    return m_symbol;
}
