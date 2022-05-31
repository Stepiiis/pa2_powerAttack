#pragma once
#include "constants.h"
#include "Map.h"
#include <iostream>
#include <memory>
#include "ncurses.h"

class Map;

class Entity {
public:
    virtual ~Entity() = default;
    Entity(int posX, int posY, int maxHealth, Map * map,int attackerID);

    // moves the entity to the given position on screen, actual showing on screen is handled by Map class
    bool move(int x, int y);

    // if the entity is alive, it will take damage and return true, otherwise it will return false
    bool takeDamage(int damage);
    // gives damage to targeted entity and if the entity dies it returns false, otherwise it returns true.
    // if the entity dies it will be removed from the map
    bool giveDamage(int damage, Entity* target);

    // removes the entity from the map
    bool destroy(); // remove from the map and sets alive to false;
    void attack();
    void update();

    void draw();
    int getHP() const;

    virtual inline char getSymbol() = 0;
    virtual Point::PointType getType() = 0;
    virtual bool checkRadius() = 0;
    size_t getID()const;
    std::pair<int,int> getPosition()const;
    void setSymbol(char symbol);

    bool printEntity(int x, int y);
    bool alive = true;
protected:
    int m_x;       // souřadnice _x

    int m_y;       // -//- y

    char m_symbol{};    // symbol identifikující typ entity

    size_t m_id;    // identifikator dane postavy

    int m_hp;      // současné zdraví

    int m_maxhp;   // maximální zdraví

    Map* m_sharedMap;   // pointer na sdilenou mapu.
                        // Mapa vždy přežije všechny entity.

    Entity* currentFocus = nullptr; // pointer to an entity that we are currently attacking

    int m_speed{};

    int m_radius{};
};