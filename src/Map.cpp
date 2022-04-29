#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include "logFile.h"
#include "Entity.h"
#include "Map.h"
#include "ncurses.h"

Map::Map()
: currentLevel(0)
{
    readMap(0);
}

/**
 * @brief Map::readMap
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
        throw("Out of bounds");
    }
    m_map[prevX][prevY] = Point(prevX, prevY, ' ');
    char symbol = entity->getSymbol();
    entity->move(x,y);
    m_map[y][x] = Point(x,y,symbol);
    mvprintw(y,x,"%c",symbol);
    return true;
}

bool Map::loadNextMap(int level){
    return   readMap(level);
}

// loads map from data files and sets up the map into a 2D array
bool Map::readMap(int level) {
    m_map.clear();
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
    int y = 0;
    while(getline(mapFile, line)) {
        if(line.length() > 0) {
            for(size_t x = 0; x<=line.length(); x++) {
                if(x==line.length()){
                    mapLines.emplace_back(x,y,'\n');
                    break;
                } else {
                    mapLines.emplace_back(x,y,line[x]);
                }
            }
            m_map.emplace_back(mapLines); // SEGFAULT HERE
            mapLines.clear();
            y++;
        }

    }
    sendToLogFile(0, "Map::readMap: Map loaded successfully", "Map.cpp");
    return true;
}


// returns const string reference to map string
void Map::printMap(std::string & map){
    for(size_t i = 0; i<m_map.size(); i++) {
        for(size_t j = 0; j<m_map[i].size(); j++) {
            map=map + m_map[i][j].m_symbol;
        }
    }
}

std::vector<std::vector<char> >Map::getEmptySpaces(){
    std::vector<std::vector<char> > emptySpaces;
    for(int row = 0; row<m_map.size(); row++) {
        for(int col = 0; col<m_map[row].size(); col++) {
            if(emptySpaces[row][col] != ' ') {
                emptySpaces[row][col] = '#';
            }
        }
    }
return emptySpaces;
}

void Map::redrawMap(){
    printMap(m_mapString);
    mvprintw(0,0,"%s",m_mapString.c_str());
}

Point::Point(int x, int y, char symbol)
: x(x), y(y), m_symbol(symbol)
{
    switch(symbol){
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
