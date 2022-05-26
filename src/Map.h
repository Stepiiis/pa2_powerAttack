#pragma once
#include <vector>
#include <string>
#include <memory>
#include <ncurses.h>
#include <functional>
#include "Entity.h"
class Entity;
class Map;
class Point {
public:
    Point(int x, int y, char symbol);
    Point() = default;
    ~Point() = default;

    enum PointType{
        Empty,
        Wall,
        Attacker,
        Tower,
        Entry,
        Exit
    };
    char m_symbol;
    int x, y;
    PointType type;
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
    explicit Map(WINDOW* win);
    // updates map according to the certain entity. its previous position will be printed as its default value
    bool updateMap(int prevX, int prevY,int x, int y, Entity * entity);
    bool updateEntity(int x, int y, Entity * entity);
    void redrawMap();
    bool loadNextMap(int level); // pass last map index as parameter
    bool readMap(int level); // pas new map index as parameter
    bool getEmptySpaces(    std::vector<std::vector<char> >& );
    void convertMap();
    void refreshWindow();
    bool checkNeighbours(int x, int y);
    std::vector<std::vector<Point> > m_map;  // 2D vector of chars
    const Point & getLaneByID(int id)const;
    void highlightLane(int lanenr);
    void highlightAttacker(int type);
    /**
    *   @abstract
     *  Procházení mapy v závislosti na tom, pro kterého attackera hledáme cestu k cíli
    *   Hledání pomocí algoritmu BFS.
    *   Tato funkce bude sice volána pokaždé když se bude rozhodovat o dalším postupu,
    *   protože se může stát, že věž na kterou byl původně attacker nasměrován je už zničena.
    *   @Nejjednodušší - jde vždy nejkratší cestou do cíle
    *   @Vyhýbání_se_věžím - pro fast attackera  najde cestu do cíle
    *   @Náběh_na_vež - běží k nejbližší věži a ničí věž dokud není mrtvá ( pro heavy attackera, který má hodně )

    */
    bool findShortestPath(Entity * entity);

    /**
     * @abstract
     * Helper function for findShortestPath BFS alogorithm
     * @param fun - function to be called on each node
     */
    template<typename T>
    bool forEachNeighbor(Point p, T Fun);
    friend Point;
protected:
    std::string m_mapString; // currently used for debugging and first drawing of the map on screen
    WINDOW* m_game_window;
    Point m_exit;
    std::vector<Point> m_entries;
    using Callback = std::function<void(Point)>;
    void forEachNeighborImpl(Point p, Callback fun);
};


