#pragma once
#include "constants.h"
#include "Map.h"
#include <iostream>
#include <memory>
#include <set>
#include "ncurses.h"

class Map;

class Entity {
public:
    virtual ~Entity() = default;
    Entity(int posX, int posY,int hp, int dmg, char symbol, int radius, int attackSpeed, Map * map,int attackerID);

    // moves the entity to the given position on screen, actual showing on screen is handled by Map class
    bool move(int x, int y);

    // if the entity is alive, it will take damage and return true, otherwise it will return false
    bool takeDamage(int damage);
    // gives damage to targeted entity and if the entity dies it returns false, otherwise it returns true.
    // if the entity dies it will be removed from the map
    bool giveDamage(int damage, Entity* target);

    // removes the entity from the map and sets alive to false
    bool destroy();
    void attack();
    void update();


    void draw();
    int getDamage() const;
    int getHP() const;
    int getCurrentFocus() const;
    char getSymbol() const;
    virtual Point::PointType getType() = 0;
    bool checkRadius(Point::PointType type);
    void calculateDeltas();
    [[nodiscard]] const std::set<std::pair<int,int>> &getDeltas() const;
    virtual std::string getTypeName() const = 0;
    bool isFocused() const;
    int getID()const;
    [[nodiscard]] std::pair<int,int> getPosition()const;

    bool printEntity(int x, int y);
    bool alive = true;
protected:
    int m_x;       // souřadnice _x

    int m_y;       // -//- y

    char m_symbol{};    // symbol identifikující typ entity

    int m_id;    // identifikator dane postavy

    int m_hp;      // současné zdraví


    Map* m_sharedMap;   // pointer na sdilenou mapu.
                        // Mapa vždy přežije všechny entity.
    std::set<std::pair<int,int>> m_deltas;
    int m_currentFocusID=-10;// ID of an entity that we are currently attacking
    bool hasFocus = false;

    int m_damage;

    int m_attackSpeed{};

    int m_radius{};
};