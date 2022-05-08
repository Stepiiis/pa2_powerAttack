#pragma once
#include <fstream>

struct defBaseEntity{
    char symbol;
    int maxHP;      // max HP points
    int range;      // range of attack
    int damage;     // damage of attack
    int speed;      // speed of attack   (1-10, 1 is default -> once per cycle, 10 is once per 10 cycles)
    int movement;   // speed of movement (1-10, 1 is default -> one move cycle, 10 is one move per 10 cycles)
    int price;      // price of unit
};


struct attackerDefs{
    defBaseEntity base;
    defBaseEntity fast;
    defBaseEntity charger;
};

struct towerDefs{
    defBaseEntity base;
    defBaseEntity fast;
    defBaseEntity highD;
    defBaseEntity slowEff;
};

class CDefinitions{
    attackerDefs _attackers;
    towerDefs _towers;
public:
    CDefinitions() = default;
    ~CDefinitions() = default;
    bool loadDefinitions();
    bool loadEntity(std::string& line, defBaseEntity & ent);
    const towerDefs & getDefender()const;
    const attackerDefs & getAttacker()const;
};

