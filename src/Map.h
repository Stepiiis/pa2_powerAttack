#pragma once

#include <vector>
#include <string>
#include <memory>
#include <ncurses.h>
#include <functional>
#include <cmath>
#include "Entity.h"
#include "Definitions.h"

#define PI 3.14159265

class Entity;

class Map;

class Point {
public:
    enum PointType {
        Empty,
        Wall,
        Attacker,
        Tower,
        Entry,
        Exit,
        Water
    };

    Point(int x, int y, char symbol);

    Point(int x, int y);

    Point(int x, int y, PointType type);

    Point() = default;

    ~Point() = default;

    Point(const Point &) = default;

    bool operator<(const Point &rhs) const;

    bool operator!=(const Point &rhs) const;

    bool operator==(const Point &rhs) const;

    Point &operator=(const Point &rhs) = default;

    char _symbol;
    int _x, _y;
    int _ident; // unique identifier for each point (used for combat)
    PointType _type;
    PointType _defaultType;
    char _defaultSymbol;
    std::map<PointType, std::map<int, bool>> _inRadiusOf;
    friend Map;
};

/**
 *  WHEN PRINTING ON SCREEN, EVERY COORDINATE SHOULD BE MOVED BY 1 TO ACCOUNT FOR THE BORDER
 */

class Map {
public:
    // draws the map specified as a string
    void printMap();

    Map() = default;

    // updates map according to the certain entity. its previous position will be printed as its default value
    bool updateMap(int x, int y,
                   Entity *entity); // used for movement. updates previous position to default value and moves intity to new position specified
    bool updateCell(int x, int y, Point::PointType type, const char symbol); // updates specific cell

    bool revertCell(int x, int y);

    bool setEntity(int x, int y, Entity *entity);

    void removeFromRadius(int x, int y, Entity *entity);

    void addToRadius(int x, int y, Entity *entity);

    bool checkBounds(int x, int y);

    bool loadNextMap(int level); // pass last map index as parameter

    bool readMap(int level = 1); // pas new map index as parameter

    Point::PointType getType(const Point &p);

    void refreshWindow(); // refreshes ncurses window

    bool checkNeighbours(int x, int y);

    bool checkClearSight(const std::pair<int, int> &p1,
                         const std::pair<int, int> &p2) const; //checks if a wall obstructs way to annother point (used for "shooting")

    bool getLaneByID(int id, Point &ret) const;

    void highlightLane(int lanenr);

    void setWindow(WINDOW *win);

    Point getMapExit();

    void clearMap();

    int getMapWidth();

    const Point &getExit() const;

    friend Point;
    std::vector<std::vector<Point> > m_map;  // 2D vector of chars
protected:
    std::string m_mapString; // currently used for debugging and first drawing of the map on screen
    WINDOW *m_game_window = nullptr;
    Point m_exit;
    std::vector<Point> m_entries;
    using Callback = std::function<void(const Point &)>;

    void forEachNeighborImpl(const Point &p, const Callback &fun); // info in cpp file
public:

/**
 * @abstract
 * Helper function for findShortestPath BFS alogorithm
 * @param fun - function to be called on each point on grid
 */
    template<typename T>
    void forEachNeighbor(const Point &p, T Fun) {
        forEachNeighborImpl(p, Fun);
    }
};


