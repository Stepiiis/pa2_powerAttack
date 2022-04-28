#pragma once
#include "constants.h"
#include <iostream>


class Entity{
public:
    Entity(int posX, int posY, int maxHealth);
    
    virtual bool move(int x, int y);

    void takeDamage(int damage);

    bool giveDamage(int damage, Entity* target);

    char getSymbol();

protected:
    size_t m_x;       // souřadnice x

    size_t m_y;       // -//- y

    char m_symbol;    // symbol identifikující typ entity

    size_t m_id;    // identifikator dane postavy

    size_t m_hp;      // současné zdraví

    uint32_t m_maxhp;   // maximální zdraví

    char color;     // barva entity
};