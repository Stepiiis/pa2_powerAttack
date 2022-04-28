#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include "logFile.h"
#include "Map.h"

Map::Map()
: currentLevel(0)
{
    readMap(0);
}
// loads map from data files and sets up the map into a 2D array
bool Map::readMap(int level) {
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
    std::vector <char> mapLines;
    while(getline(mapFile, line)) {
        mapLines.clear();
        if(line.length() > 0) {
            for(size_t i = 0; i<=line.length(); i++) {
                if(i==line.length()){
                    mapLines.push_back('\n');
                    break;
                } else {
                    mapLines.emplace_back(line[i]);
                }
            }
            m_map.emplace_back(mapLines);
        }
    }

    std::cout << "Map loaded" << std::endl;
    return true;
}


// returns const string reference to map string
void Map::printMap(std::string & map){
    for(size_t i = 0; i<m_map.size(); i++) {
        for(size_t j = 0; j<m_map[i].size(); j++) {
            map=map + m_map[i][j];
        }
    }
}
