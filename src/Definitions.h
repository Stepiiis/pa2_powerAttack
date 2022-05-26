#pragma once
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include "constants.h"
using defEntity = std::map<std::string,std::map<std::string,int> >; // entity definitions map containing all values

struct defBaseEntity{
    char symbol;
    int maxHP;      // max HP points
    int range;      // range of attack
    int damage;     // damage of attack
    int speed;      // speed of attack   (1-10, 1 is default -> once per cycle, 10 is once per 10 cycles)
    int movement;   // speed of movement (1-10, 1 is default -> one move cycle, 10 is one move per 10 cycles)
    int price;      // price of unit
};


struct attackerDef{
    defBaseEntity base;
    defBaseEntity fast;
    defBaseEntity charger;
};

struct towerDef{
    defBaseEntity base;
    defBaseEntity fast;
    defBaseEntity highD;
    defBaseEntity slowEff;
};


class CDefinitions{
        defEntity _attackerDefinitions;
        defEntity _towerDefinitions;
public:
    CDefinitions() = default;
    ~CDefinitions() = default;
    bool loadDefinitions(); // helper func to facilitate all def. loadings
    bool loadEntity(std::string& path, defEntity& entity); // loads deifinition for given entity
//    bool convertDefinitions();
    const defEntity & getTower()const;
    const defEntity & getAttacker()const;
};


