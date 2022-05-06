#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Entity.h"
class Entity;
class Map;
class Point {
public:
    enum PointType{
        Empty,
        Wall,
        Attacker,
        Tower,
        Entry,
        Exit
    };
    Point(int x, int y, char symbol);
    char m_symbol;
    int x, y;
    PointType type;
    friend Map;
};

class Map
{
public:
    // draws the map specified as a string
    void printMap(std::string& map);
    Map() = default;
    // updates map according to the certain entity. its previous position will be printed as its default value
    bool updateMap(int prevX, int prevY,int x, int y, Entity * entity);
    void redrawMap();
    bool loadNextMap(int level); // pass last map index as parameter
    bool readMap(int level); // pas new map index as parameter
    bool getEmptySpaces(    std::vector<std::vector<char> >& );
    void convertMap();
    std::vector<std::vector<Point> > m_map;  // 2D vector of chars
    friend Point;
protected:
    std::string m_mapString;
    int currentLevel{};
};


/*
 * Procházení mapy v závislosti na tom, pro kterého attackera hledáme cestu k cíli
 * Nejjednodušší - jde explicitně do cíle
 * Vyhýbání se věžím - pro fast attackera  najde cestu do cíle
 * Náběh na vež - běží k nejbližší věži a ničí věž dokud není mrtvá ( pro heavy attackera, který má hodně )
 */