#pragma once
#include <vector>
#include <string>
#include <memory>
#include <ncurses.h>
#include <functional>
#include "Entity.h"
#include "Definitions.h"
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
    Point(int x, int y);
    Point(int x, int y, PointType type);
    Point() = default;
    ~Point() = default;
    Point(const Point&);
    bool operator<(const Point& rhs) const;
    bool operator!=(const Point& rhs) const;
    bool operator==(const Point& rhs) const;
    Point& operator=(const Point& rhs);

    char _symbol;
    int _x, _y;
    size_t _IDent; // unique identifier for each point (used for combat)
    PointType _type;
    PointType _defaultType;
    char _defaultSymbol;
    friend Map;
};

/**
 * EVERY COORDINATE SHOULD BE MOVED BY 1 TO ACCOUNT FOR THE BORDER
 */

class Map
{
public:
    // draws the map specified as a string
    void printMap();
    Map() = default;
    // updates map according to the certain entity. its previous position will be printed as its default value
    bool updateMap(int x, int y, Entity * entity); // used for movement. updates previous position to default value and moves intity to new position specified
    bool updateCell(int x, int y, Point::PointType type, const char symbol); // updates specific cell
    bool revertCell(int x, int y);
    bool setEntity(int x, int y, Entity * entity);
    void redrawMap();
    bool loadNextMap(int level); // pass last map index as parameter
    bool readMap(int level); // pas new map index as parameter
    Point::PointType getType(const Point& p);
    void convertMap();
    void refreshWindow();
    bool checkNeighbours(int x, int y);
    bool getLaneByID(int id, Point & ret)const;
    void highlightLane(int lanenr);
    void drawLanes();
    void setWindow(WINDOW * win);
    Point getMapExit();
    bool getEmptySpaces(    std::vector<std::vector<char> >& );
    void clearMap();
    int getMapWidth();

    friend Point;
    std::vector<std::vector<Point> > m_map;  // 2D vector of chars
protected:
    std::string m_mapString; // currently used for debugging and first drawing of the map on screen
    WINDOW* m_game_window = nullptr;
    Point m_exit;
    std::vector<Point> m_entries;
    using Callback = std::function<void(const Point&)>;
    /**
    *   @abstract
    *   Procházení mapy v závislosti na tom, pro kterého attackera hledáme cestu k cíli
    *   Hledání pomocí algoritmu BFS.
    *   Tato funkce bude sice volána pokaždé když se bude rozhodovat o dalším postupu,
    *   protože se může stát, že věž na kterou byl původně attacker nasměrován je už zničena.
    *   @Nejjednodušší - jde vždy nejkratší cestou do cíle
    *   @Vyhýbání_se_věžím - pro fast attackera najde cestu do cíle která bude nejkratší dobu v dosahu věží
    *   @Náběh_na_vež - běží k nejbližší věži a ničí věž dokud není mrtvá ( pro heavy attackera, který má hodně hp)
    */
    void forEachNeighborImpl(const Point& p, Callback fun);
public:
    /**
 * @abstract
 * Helper function for findShortestPath BFS alogorithm
 * @param fun - function to be called on each node
 */
    template<typename T>
    void forEachNeighbor(const Point& p, T Fun){
        forEachNeighborImpl(p,Fun);
    }
};


