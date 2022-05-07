#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include "logFile.h"
#include "Entity.h"
#include "Map.h"
#include "ncurses.h"

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
}

Map::Map(WINDOW* win)
: m_game_window(win)
{}


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
    m_map[prevY][prevX] = Point(prevX, prevY, ' ');
    mvwprintw(m_game_window,prevX+1,prevY+1,"%c"," ");
    char symbol = entity->getSymbol();
    entity->move(x,y);
    m_map[y][x] = Point(x,y,symbol);
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
//                if(x==line.length()){
//                    mapLines.emplace_back(x,y,'\n');
//                    break;
//                } else {
                    mapLines.emplace_back(x,y,line[x]);
//                }
            }
            m_map.push_back(mapLines); // SEGFAULT HERE
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


// returns const string reference to map string
void Map::printMap(){
    for(const auto& line : m_map) {
        for (const auto &point: line) {
            mvwprintw(m_game_window, point.y+1, point.x+1, "%c", point.m_symbol);
        }
    }
    wrefresh(m_game_window);
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

bool Map::findShortestPath(Entity *entity) {
    return false;
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

