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
    for(const auto& line : m_map) {
        for (const auto &point: line) {
            mvwprintw(m_game_window, point.y+1, point.x+1, "%c", point.m_symbol);
            if(point.x == 0){
                if(point.m_symbol == '<'){
                    m_exit = point;
                }
            }
            else if(point.x == line.back().x-3) {
                if(point.m_symbol == '<'){
                    m_entries.emplace_back(point.x,point.y,point.m_symbol);
                }
            }
        }
    }
    wrefresh(m_game_window);
}
const Point & Map::getLaneByID(int id)const{
    return m_entries.at(id);
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

void highlightAttacker(int type){
    // todo highlight attacker according to definition file
}