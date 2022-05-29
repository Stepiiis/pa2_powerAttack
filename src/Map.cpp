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
        : x(x), y(y), m_symbol(symbol)
{
    switch (symbol) {
        case '#':
            type = Wall;
            break;
        case ' ':
            type = Empty;
            break;
        case '@':
            type = Attacker;
            break;
        case 'I':
            type = Tower;
            break;
        case '<':
            type = Entry;
            break;
        case '>':
            type = Exit;
            break;
    }
    IDent = 0;
}
// helper constructor for ForEachNeighbour function
Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
    type = Empty;
    m_symbol = ' ';
    IDent = 0;

}

bool Point::operator<(const Point &rhs) const {
    return std::tie(x, y) < std::tie(rhs.x, rhs.y);
}

bool Point::operator!=(const Point &rhs) const {
    return x != rhs.x || y != rhs.y;
}

bool Point::operator==(const Point&rhs) const {
    return x == rhs.x && y == rhs.y;
}

Point::Point(const Point & src) {
    x=src.x;
    y=src.y;
    type=src.type;
    m_symbol=src.m_symbol;
    IDent=src.IDent;
}

Point &Point::operator=(const Point &rhs) {
    x=rhs.x;
    y=rhs.y;
    type=rhs.type;
    m_symbol=rhs.m_symbol;
    IDent=rhs.IDent;
    return *this;
}

Point::Point(int x, int y, PointType type)
: x(x), y(y), type(type)
{
}

/**
 * EVERY COORDINATE SHOULD BE MOVED BY 1 TO ACCOUNT FOR THE BORDER
 */

/**
 * @brief Map::updateMap
 * @param prevX  previous x position which will be filled with empty space
 * @param prevY  previous y position which will be filled with empty space
 * @param x      new x position to move entity to
 * @param y      new y position to move entity to
 * @param entity entity to move
 */
bool Map::updateMap(int prevX, int prevY, int x, int y, Entity * entity)
{
    if(y>m_map.size() || (y<0) || (x>m_map[0].size()) || (x<0))
    {
        throw(mapException("Out of bounds"));
    }
    m_map[prevY][prevX].m_symbol= ' ';
    m_map[prevY][prevX].IDent=0;
    mvwprintw(m_game_window,prevY+1,prevX+1,"%c",' ');
    char symbol = entity->getSymbol();
    entity->move(x,y);
    m_map[y][x] = Point(x,y,symbol);
    m_map[y][x].IDent = entity->getID();
    mvwprintw(m_game_window,y+1,x+1,"%c",symbol);
    return true;
}

bool Map::updateEntity(int x, int y, Entity * entity){
    if(y>m_map.size() || (y<0) || (x>m_map[0].size()) || (x<0))
    {
        throw(mapException("Out of bounds"));
    }
    m_map[y][x] = Point(x,y,entity->getSymbol());
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
            m_map.push_back(mapLines);
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
            mvwprintw(m_game_window, point.y+1, point.x+1, "%c", point.m_symbol);
            if(point.x == 0 || point.x == 1){
                if(point.m_symbol == '<'){
                    m_exit = point;
                    point.type = Point::Exit;
                }else if(point.m_symbol == '='){
                    point.type = Point::Exit;
                }
            }
            else if(point.x == line.back().x-3) {
                if(point.m_symbol == '<'){
                    m_entries.emplace_back(point.x,point.y,point.m_symbol);
                    point.type = Point::Entry;
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
            mvwprintw(m_game_window,m_entries[i].y+1,m_entries[i].x+1,"%c",'<');
            mvwprintw(m_game_window,m_entries[i].y+1,m_entries[i].x+2,"%c",'=');
            mvwprintw(m_game_window,m_entries[i].y+1,m_entries[i].x+3,"%d",i+1);
            wattroff(m_game_window, style);
            wrefresh(m_game_window);
        }else{
            mvwprintw(m_game_window,m_entries[i].y+1,m_entries[i].x+1,"%c",'<');
            mvwprintw(m_game_window,m_entries[i].y+1,m_entries[i].x+2,"%c",'=');
            mvwprintw(m_game_window,m_entries[i].y+1,m_entries[i].x+3,"%d",i+1);
            wrefresh(m_game_window);
        }
    }
}

void Map::drawLanes(){
    for(int i = 0; i<m_entries.size(); i++){
        mvwprintw(m_game_window,m_entries[i].y+1,m_entries[i].x+1,"%c",'<');
        mvwprintw(m_game_window,m_entries[i].y+1,m_entries[i].x+2,"%c",'=');
        mvwprintw(m_game_window,m_entries[i].y+1,m_entries[i].x+3,"%d",i+1);
        wrefresh(m_game_window);
    }
}

bool Map::getEmptySpaces(    std::vector<std::vector<char> > &_emptySpaces){
_emptySpaces.erase(_emptySpaces.begin(), _emptySpaces.end());
std::vector<char> lines;
    for(int row = 0; row<m_map.size(); row++) {
        lines.clear();
        for(int col = 0; col<m_map[row].size(); col++) {
            if(m_map[row][col].m_symbol != ' ') {
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
            m_mapString.push_back(m_map[row][col].m_symbol);
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
        if(m_map[y][x - 1].type != Point::Entry && m_map[y][x + 1].type != Point::Entry && m_map[y - 1][x].type != Point::Entry && m_map[y + 1][x].type != Point::Entry){
            if(m_map[y-1][x - 1].type != Point::Entry && m_map[y-1][x + 1].type != Point::Entry && m_map[y - 1][x+1].type != Point::Entry && m_map[y + 1][x-1].type != Point::Entry){
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
        x = p.x + xd;
        y = p.y + yd;
        if (y < 0 || y >= (int)m_map.size()) continue;
        auto& row = m_map[y];
        if (x < 0 || x >= (int)row.size()) continue;
        fun(m_map[y][x]);
//        sendToLogFile(0, "currentPoint"+std::to_string(x)+" "+std::to_string(y), "BFS debug");
//        mvwprintw(m_game_window,y+1,x+1,"%c",'V');
//        wtimeout(m_game_window,20);
//        wgetch(m_game_window);
//        wrefresh(m_game_window);
    }
}

Point::PointType Map::getType(const Point& p) {
    return m_map[p.y][p.x].type;
}

Point Map::getMapExit() {
    return m_exit;
}
