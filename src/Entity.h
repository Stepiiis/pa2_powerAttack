#pragma once
#include "constants.h"
#include "Map.h"
#include <iostream>
#include <memory>

class Map;

class Entity {
public:
    ~Entity() = default;
    Entity(int posX, int posY, int maxHealth, std::shared_ptr<Map> map);
    
    bool move(int x, int y);

    void takeDamage(int damage);

    bool giveDamage(int damage, Entity* target);

    bool destroy(); // remove from the map and sets alive to false;
    void attack();
    void update();

    void draw();
    int getHP() const;

    virtual inline char getSymbol() = 0;
    virtual bool checkRadius() = 0;
    void setSymbol(char symbol);

    bool printEntity(int x, int y);
    bool alive = true;
protected:
    size_t m_x;       // souřadnice x

    size_t m_y;       // -//- y

    char m_symbol;    // symbol identifikující typ entity

    size_t m_id;    // identifikator dane postavy

    size_t m_hp;      // současné zdraví

    uint32_t m_maxhp;   // maximální zdraví

    std::shared_ptr<Map> _map;

    int _speed;

    int m_radius;

    short m_color;     // barva entity
};