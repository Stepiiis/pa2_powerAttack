#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include "logFile.h"
#include "Entity.h"
#include "Map.h"
#include "ncurses.h"
#include <complex>

Point::Point(int x, int y, char symbol)
        : _x(x), _y(y), _symbol(symbol), _defaultSymbol(symbol)
{
    switch (symbol) {
        case '#':
            _type = Wall;
            break;
        case ' ':
            _type = Empty;
            break;
        case '@':
            _type = Attacker;
            break;
        case 'I':
            _type = Tower;
            break;
        case '<':
            _type = Entry;
            break;
        case '>':
            _type = Exit;
            break;
    }
    _defaultType = _type;
    _IDent = 0;
}
// helper constructor for ForEachNeighbour function
Point::Point(int x, int y) {
    this->_x = x;
    this->_y = y;
    _type = Empty;
    _symbol = ' ';
    _IDent = 0;

}

bool Point::operator<(const Point &rhs) const {
    return std::tie(_x, _y) < std::tie(rhs._x, rhs._y);
}

bool Point::operator!=(const Point &rhs) const {
    return _x != rhs._x || _y != rhs._y;
}

bool Point::operator==(const Point&rhs) const {
    return _x == rhs._x && _y == rhs._y;
}

Point::Point(const Point & src) {
    _x=src._x;
    _y=src._y;
    _type=src._type;
    _defaultType=src._defaultType;
    _symbol = src._symbol;
    _defaultSymbol = src._defaultSymbol;
    _IDent=src._IDent;
}

Point &Point::operator=(const Point &rhs) {
    _x=rhs._x;
    _y=rhs._y;
    _type=rhs._type;
    _defaultType=rhs._defaultType;
    _symbol = rhs._symbol;
    _defaultSymbol = rhs._defaultSymbol;
    _IDent=rhs._IDent;
    return *this;
}

Point::Point(int x, int y, PointType type)
: _x(x), _y(y), _type(type)
{
}

/**
 * EVERY COORDINATE SHOULD BE MOVED BY 1 TO ACCOUNT FOR THE BORDER
 */

/**
 * @brief Map::updateMap
 * @param prevX  previous _x position which will be filled with empty space
 * @param prevY  previous y position which will be filled with empty space
 * @param x      new _x position to move entity to
 * @param y      new y position to move entity to
 * @param entity entity to be moved
 */
bool Map::updateMap( int x, int y, Entity * entity)
{
    if(y>m_map.size() || (y<0) || (x>m_map[0].size()) || (x<0))
    {
        throw(mapException("Out of bounds"));
    }
    auto prev = entity->getPosition();
    m_map[prev.second][prev.first]._symbol = m_map[prev.second][prev.first]._defaultSymbol;
    m_map[prev.second][prev.first]._type = m_map[prev.second][prev.first]._defaultType;
    m_map[prev.second][prev.first]._IDent=0;
    mvwprintw(m_game_window,prev.second+1,prev.first+1,"%c",m_map[prev.second][prev.first]._defaultSymbol);
    char symbol = entity->getSymbol();
    entity->move(x,y);
    m_map[y][x]._type = entity->getType();
    m_map[y][x]._symbol = symbol;
    m_map[y][x]._IDent = entity->getID();
    mvwprintw(m_game_window,y+1,x+1,"%c",symbol);
    return true;
}

bool Map::setEntity(int x, int y, Entity * entity){
    if(y>m_map.size() || (y<0) || (x>m_map[0].size()) || (x<0))
    {
        throw(mapException("Out of bounds"));
    }
    m_map[y][x]._type = entity->getType();
    m_map[y][x]._symbol = entity->getSymbol();
    m_map[y][x]._IDent = entity->getID();
    entity->move(x,y);
    mvwprintw(m_game_window,y+1,x+1,"%c",entity->getSymbol());
    return true;
}

bool Map::loadNextMap(int level){
    return   readMap(level);
}

void Map::refreshWindow(){
    wrefresh(m_game_window);
}


// loads map from data files and sets up the map into a 2D array
bool Map::readMap(int level) {
    m_map.erase(m_map.begin(), m_map.end());
    m_map.reserve(30);

    std::string mapLocation = "data/maps/map" + std::to_string(level) + ".txt";
    std::ifstream mapFile(mapLocation, std::ios::in);
    if(!mapFile.is_open()) {
        std::cout << "Could not open map file :" << mapLocation << std::endl;
        try {
            sendToLogFile(10, "Map::readMap: Could not open map file: " + mapLocation, "Map.cpp");
        } catch (logException &e) {
            std::cout << e.what() << std::endl;
        }
        return false;
    }
    std::string line;
    std::vector <Point> mapLines;
    auto it = m_map.begin();
    int y = 0;
    while(getline(mapFile, line)) {
        if(line.length() > 0) {
            m_map[y].reserve(100);
            for(size_t x = 0; x<=line.length(); x++) {
                mapLines.emplace_back(x,y,line[x]);
            }
            m_map.emplace_back(mapLines);
            mapLines.clear();
            y++;
        }
    }
//    std::string mapa;
//    printMap(mapa);
//    std::cout << mapa<< std::endl;
    mapFile.close();
    sendToLogFile(0, "Map::readMap: Map loaded successfully", "Map.cpp");
    return true;
}


// Prints the map to the screen and also checks for inputs and exits from the map
void Map::printMap(){
    for( auto& line : m_map) {
        for ( auto & point: line) {
            mvwprintw(m_game_window, point._y + 1, point._x + 1, "%c", point._symbol);
            if(point._x == 0 || point._x == 1){
                if(point._symbol == '<'){
                    m_exit = point;
                    point._type = Point::Exit;
                    point._defaultType = Point::Exit;
                }else if(point._symbol == '='){
                    point._type = Point::Exit;
                    point._defaultType = Point::Exit;
                }
            }
            else if(point._x == line.back()._x - 3 || point._x == line.back()._x - 2){
                if(point._symbol == '<'){
                    m_entries.emplace_back(point._x, point._y, point._symbol);
                    point._type = Point::Entry;
                    point._defaultType = Point::Entry;
                }else if(point._symbol == '='){
                    point._type = Point::Entry;
                    point._defaultType = Point::Entry;
                }
            }
        }
    }
    wrefresh(m_game_window);
}
bool Map::getLaneByID(int id, Point & spawnLane)const{
    if(m_entries.size()<id)
        return false;
    spawnLane = m_entries.at(id);
    return true;
}

void Map::highlightLane(int lanenr){
    auto style = A_STANDOUT;
    for(int i = 0; i<m_entries.size(); i++){
        if(i==lanenr){
            wattron(m_game_window, style);
            mvwprintw(m_game_window, m_entries[i]._y + 1, m_entries[i]._x + 1, "%c", '<');
            mvwprintw(m_game_window, m_entries[i]._y + 1, m_entries[i]._x + 2, "%c", '=');
            mvwprintw(m_game_window, m_entries[i]._y + 1, m_entries[i]._x + 3, "%d", i + 1);
            wattroff(m_game_window, style);
            wrefresh(m_game_window);
        }else{
            mvwprintw(m_game_window, m_entries[i]._y + 1, m_entries[i]._x + 1, "%c", '<');
            mvwprintw(m_game_window, m_entries[i]._y + 1, m_entries[i]._x + 2, "%c", '=');
            mvwprintw(m_game_window, m_entries[i]._y + 1, m_entries[i]._x + 3, "%d", i + 1);
            wrefresh(m_game_window);
        }
    }
}

void Map::drawLanes(){
    for(int i = 0; i<m_entries.size(); i++){
        mvwprintw(m_game_window, m_entries[i]._y + 1, m_entries[i]._x + 1, "%c", '<');
        mvwprintw(m_game_window, m_entries[i]._y + 1, m_entries[i]._x + 2, "%c", '=');
        mvwprintw(m_game_window, m_entries[i]._y + 1, m_entries[i]._x + 3, "%d", i + 1);
        wrefresh(m_game_window);
    }
}

bool Map::getEmptySpaces(    std::vector<std::vector<char> > &_emptySpaces){
_emptySpaces.erase(_emptySpaces.begin(), _emptySpaces.end());
std::vector<char> lines;
    for(int row = 0; row<m_map.size(); row++) {
        lines.clear();
        for(int col = 0; col<m_map[row].size(); col++) {
            if(m_map[row][col]._symbol != ' ') {
                lines.emplace_back('#');
            }else{
                lines.emplace_back(' ');
            }
        }
        _emptySpaces.push_back(lines);
    }
    return true;
}

void Map::convertMap() {
    for(int row = 0; row<m_map.size(); row++) {
        for(int col = 0; col<m_map[row].size(); col++) {
            m_mapString.push_back(m_map[row][col]._symbol);
        }
    }
}

void Map::redrawMap(){
    convertMap();
    printMap();
//    mvwprintw(m_game_window,0,0,"%s",m_mapString.c_str());
}


bool Map::checkNeighbours(int x, int y){
    if(x > 0 && x < MAP_WIDTH - 1 && y > 0 && y < MAP_HEIGHT - 1){
        if(m_map[y][x - 1]._type != Point::Entry && m_map[y][x + 1]._type != Point::Entry && m_map[y - 1][x]._type != Point::Entry && m_map[y + 1][x]._type != Point::Entry){
            if(m_map[y-1][x - 1]._type != Point::Entry && m_map[y - 1][x + 1]._type != Point::Entry && m_map[y - 1][x + 1]._type != Point::Entry && m_map[y + 1][x - 1]._type != Point::Entry){
                return true;
            }
        }
    }
    return false;
}

void Map::setWindow(WINDOW *win) {
    m_game_window=win;
}

void Map::forEachNeighborImpl(const Point &p, Map::Callback fun) {
    int x, y;
    for (auto [xd, yd] : { std::pair<int, int>{-1,0}, {0,-1}, {1, 0}, {0, 1} }) {
        x = p._x + xd;
        y = p._y + yd;
        if (y < 0 || y >= (int)m_map.size()) continue;
        auto& row = m_map[y];
        if (x < 0 || x >= (int)row.size()) continue;
        fun(m_map[y][x]);
    }
}

Point::PointType Map::getType(const Point& p) {
    return m_map[p._y][p._x]._type;
}

Point Map::getMapExit() {
    return m_exit;
}

bool Map::updateCell(int x, int y, Point::PointType type, const char symbol) {
    if (x < 0 || x >= (int)m_map[y].size()) throw mapException("Invalid _x coordinate: updateCell");
    if (y < 0 || y >= (int)m_map.size()) throw mapException("Invalid y coordinate: updateCell");
    m_map[y][x]._type = type;
    m_map[y][x]._symbol = symbol;
    mvwprintw(m_game_window,y+1,x+1,"%c",symbol);
    return true;
}

bool Map::revertCell(int x, int y) {
    if (x < 0 || x >= (int)m_map[y].size()) throw mapException("Invalid _x coordinate: updateCell");
    if (y < 0 || y >= (int)m_map.size()) throw mapException("Invalid y coordinate: updateCell");
    m_map[y][x]._type = m_map[y][x]._defaultType;
    m_map[y][x]._symbol = m_map[y][x]._defaultSymbol;
    mvwprintw(m_game_window,y+1,x+1,"%c",m_map[y][x]._defaultSymbol);
    return true;
}

void Map::clearMap() {
//    for(auto &row : m_map) {
//       row.clear();
//    }
    m_map.clear();
}

int Map::getMapWidth() {
    int width = 0;
    for(auto &row : m_map) {
        if(row.size() > width) {
            width = (int)row.size();
        }
    }
    return width;
}
