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

    /// Draws the map 2D array onto screen and saves where the entries and exits are.
    void printMap();

    /// default destructor because no heap allocation was done manually
    Map() = default;


    /**
    * @Brief
    *        Updates map according to the certain entity. its previous position will be printed as its default value
    *
    * @param prevX  previous _x position which will be filled with empty space.
    * @param prevY  previous y position which will be filled with empty space.
    * @param x      new _x position to move entity to.
    * @param y      new y position to move entity to.
    * @param entity entity to be moved.
    */
    bool updateMap(int x, int y, Entity *entity);

    /**
     * Udates specific cell according to given parameters
     *
     * @param x         x position to be edited.
     * @param y         y position to be edited.
     * @param type      type of point to be edited.
     * @param symbol    symbol to be printed on the position.
     */
    bool updateCell(int x, int y, Point::PointType type, char symbol);


    /**
     * Reverts state of cell to its default value which was saved upon loading of map file
     * @param x     position to be reversed.
     * @param y     position to be reversed.
     * */
    bool revertCell(int x, int y);

    /**
     * Prints the entity on the Map and edits parameters on given coordinate.
     * @param x         coordinate to be edited.
     * @param y         coordinate to be edited.
     * @param entity    entity which will be used as reference.
     */
    bool setEntity(int x, int y, Entity *entity);

    /**
     * Removes given entity's reach on point.
     * @param x      coordinate to be edited
     * @param y      coordinate to be edited
     * @param entity entity whos reach should be edited on given point
     */
    void removeFromRadius(int x, int y, Entity *entity);

    /**
     * Adds entitys reach to given point.
     * @param x coordinate to be edited
     * @param y coordinate to be edited
     * @param entity entity given
     */
    void addToRadius(int x, int y, Entity *entity);

    /**
     * Checks if the coordinateis within bounds of the map.
     * @returns true if it is within boudns and false if not.
     * */
    bool checkBounds(int x, int y);

    /**
     * ** Unused **
     * loads next map whose level is specified in the parameter
     * @param level map to be loaded
     * */
    bool loadNextMap(int level); // pass last map index as parameter

    /**
     * loads next map whose level is specified in the parameter
     * @param level map to be loaded
     * */
    bool readMap(int level = 1); // pas new map index as parameter

    /**
     * @returns PointType of point passed as argument
     * @param p point to be checked
     */
    Point::PointType getType(const Point &p);

    /** refreshes ncurses game window   (m_game_window) which is set by setWindow() */
    void refreshWindow();

    /** checks if neighbours are not Entry or Exit */
    bool checkNeighbours(int x, int y);

    /**
     * @Brief
     * Function tries to find a clear path from p1 to p2.
     * Used by entities to check if they can clearly see another entity that they want to attack.
     *
     * @context The function uses my own algorithm, which decides based on the degree between two points
     *  because I didnt fully understand bresenham algorithm i used my much slower version but atleast i didnt copypaste it
     *
     * @param p1 start point coordinate
     * @param p2 end point coordinate
     * @return true if path was found and false if there is an obstruction
     */
    bool checkClearSight(const std::pair<int, int> &p1,
                         const std::pair<int, int> &p2) const; //checks if a wall obstructs way to annother point (used for "shooting")

    /**
     * @returns sets ret value to given spawnLane coordinate according to loaded entries vector
     * @param id is the id of lane that i want to set as spawnLane to the ret value
     * @param ret value of spawnLane to be edited
     * @returns trueif value id is within number of entries. otherwise false
     */
    bool getLaneByID(int id, Point &ret) const;

    /**
     * highlights given lane on screen according to the parameter passed
     * @param lanenr number of lane to be highlighted on screen
     */
    void highlightLane(int lanenr);

    /**
     * sets m_game_screen to given value
     * @param win pointer to ncurses WINDOW
     * */
    void setWindow(WINDOW *win);

   /// @returns exit out of the map
    Point getMapExit();

    /// clears and deletes whole map file from 2D vector
    void clearMap();

    /// @returns largest x value on the map
    int getMapWidth();

    /// @returns read only reference to the Point of exit out of the map
    const Point &getExit() const;

    friend Point;
    /// 2D vector of Points which is used as map grid
    std::vector<std::vector<Point> > m_map;
protected:
    std::string m_mapString; // currently used for debugging and first drawing of the map on screen
    WINDOW *m_game_window = nullptr;
    Point m_exit;
    std::vector<Point> m_entries;
    using Callback = std::function<void(const Point &)>;

    /**
    *   @briefEN
    *   finding the shortest path toa target depends on who the function is called because each entity has its own target\n
    *   Uses BFS algorithm.\n
    *   This function is called every time a next move is supposed to be made,
    *   because the path could be calculated to an already dead entity or map had otherwise changed \n
    *   @Basic - finds shortest path to end\n
    *   @Avoiding_tower_perimeters -fastAttacker finds shortest path to exit while avoindng taking damage from Towers\n
    *   @Charge_On_Tower- charges toward the closest Tower or Exit(depending on which is closer)\n
    */
    /**
    *   @briefCZ
    *   Procházení mapy v závislosti na tom, pro kterého attackera hledáme cestu k cíli
    *   Hledání pomocí algoritmu BFS.
    *   Tato funkce bude sice volána pokaždé když se bude rozhodovat o dalším postupu,
    *   protože se může stát, že věž na kterou byl původně attacker nasměrován je už zničena.
    *   @Nejjednodušší - jde vždy nejkratší cestou do cíle
    *   @Vyhýbání_se_věžím - pro fast attackera najde cestu do cíle která bude nejkratší dobu v dosahu věží
    *   @Náběh_na_vež - běží k nejbližší věži a ničí věž dokud není mrtvá ( pro heavy attackera, který má hodně hp)
    */
    void forEachNeighborImpl(const Point &p, const Callback &fun); // info in cpp file
public:

/**
 * @abstract
 * Helper function for findShortestPath BFS alogorithm
 * @param fun - function to be called on each point on grid (easier to be specified by lambda in the final function so the lambda can catch by reference and access all variables inside the function who calls it)
 */
    template<typename T>
    void forEachNeighbor(const Point &p, T Fun) {
        forEachNeighborImpl(p, Fun);
    }
};


