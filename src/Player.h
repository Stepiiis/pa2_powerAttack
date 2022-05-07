#pragma once
#include "Entity.h"
#include "Attacker.h"
#include <memory>
#include <vector>
#include "Map.h"
class Player{
public:
    Player(Map* map, int baseHp, int range, int damage);
    Player(const Player&);
    Player& operator=(const Player&);
    virtual ~Player() = default;
protected:
    int _score;    // score calculated at the end of the game. could be calculated in class CGame
    int _currency; // current amount of money player has available to buy attackers
    int _baseHp;   // base hp of attackers
    int _baseRange; // base range of attackers
    int _baseAtk;   // base damage of attackers
    Map* _map;
    std::vector<std::unique_ptr<Attacker> > _attackers;
};
