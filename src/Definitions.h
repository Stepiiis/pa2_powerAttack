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
    /** loads all definitions onto two instances of structure defEntity @returns true if definitions went correctly. otherwise returns false.
     * Can throw synException if fatal error occurs.*/
    bool loadDefinitions();

    /**loads definition for given entity. @returns true if entity definition went cirrectly. otherwise returns false.
     * Can throw synException if fatal error occurs.*/
    bool loadEntity(std::string& path, defEntity& entity);  // loads deifinition for given entity

    /** @reutrns read reference to Tower definitions.*/
    const defEntity & getTower();
    
    /** returns read reference to Attacker definitions.*/
    const defEntity & getAttacker();
};


