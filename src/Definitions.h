#pragma once
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include "constants.h"
#include "logFile.h"

using defEntity = std::map<std::string,std::map<std::string,int> >; // entity definitions map containing all values


class CDefinitions{
private:
    defEntity _attackerDefinitions;
    defEntity _towerDefinitions;
public:
    CDefinitions() = default;
    ~CDefinitions() = default;
    bool loadDefinitions();                                 // helper func to facilitate all def. loadings
    bool loadEntity(std::string& path, defEntity& entity);  // loads deifinition for given entity
    const defEntity & getTower();
    const defEntity & getAttacker();
};


