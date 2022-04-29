#pragma once
#include <vector>
#include <string>
class Map
{
public:
    void printMap(std::string& map);
    explicit Map();
    bool updateMap(int x, int y, Entity * entity);
    void redrawMap();
        bool loadNextMap(int level); // pass last map index as parameter
    bool readMap(int level); // pas new map index as parameter
    class Point {
    private:
        friend Map;
        Point(int x, int y);
        int x, y;
    };

    std::vector<std::vector<char> > m_map;  // 2D vector of chars
    std::string m_mapString;
    int currentLevel;
};

enum class PointType{
    Empty,
    Wall,
    Entity
};