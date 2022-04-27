#pragma once
#include "constants.h"
class Entity{
public:
    Entity(int posX, int posY, uint maxHealth);
    
    virtual bool move(int x, int y);

    void takeDamage(int damage);

    bool giveDamage(int damage, Entity* target);
        
protected:
    uint m_x;       // souřadnice x

    uint m_y;       // -//- y

    char symbol;    // symbol identifikující typ entity

    size_t m_id;    // identifikator dane postavy

    uint m_hp;      // současné zdraví

    uint m_maxhp;   // maximální zdraví

    char color;     // barva entity
};