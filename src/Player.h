#pragma once
#include "Entity.h"
#include "Attacker.h"
#include <memory>
#include <vector>
#include "Map.h"
class Player{
public:
    Player(Map& map, int baseHp, int range, int damage);
    Player(const Player&);
    Player& operator=(const Player&);
    ~Player() = default;
protected:
    int _score;
    int _currency;
    int _baseHp;
    int _baseRange;
    int _baseAtk;
    std::shared_ptr<Map> _map;
    std::vector<std::unique_ptr<Attacker> > _attackers;
};
