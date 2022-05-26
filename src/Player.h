#pragma once
#include "Entity.h"
#include "Attacker.h"
#include "Definitions.h"
#include <memory>
#include <vector>
#include "Map.h"

class Player{
public:
    Player(Map* map, defEntity def);
    Player(const Player&) = default;
    Player& operator=(const Player&);
    virtual ~Player() = default;
    void setLane(int lane);
    bool spawnAttacker(int type); // 0 = basic, 1 = fast, 2 = charger
    void setCoins(int coins);
    int getCoins();
protected:
    int _score;    // score calculated at the end of the game. could be calculated in class CGame
    int _coins;    // current amount of money player has available to buy attackers
    Point _spawnLane;
    Map* _map;
    defEntity _def; // definitions of towers loaded from file
    std::vector< Attacker* > _attackers;
};
