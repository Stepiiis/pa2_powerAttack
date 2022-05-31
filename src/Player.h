#pragma once
#include "Entity.h"
#include "Attacker.h"
#include "Definitions.h"
#include <memory>
#include <vector>
#include "Map.h"
#include <algorithm>
#include <queue>

class Player{
public:
    Player(Map* map, defEntity def);
    Player(const Player&) = default;
    Player& operator=(const Player&);
    virtual ~Player() = default;
    void setLane(int lane);
    void setAttackerType(int type); // same as spawn
    void addAttackerToQueue();
    bool spawnAttacker(); // 0 = basic, 1 = fast, 2 = charger
    void moveAttackers();
    bool emptyAttackers();
    bool emptyAttackerQueue();
    void setCoins(int coins);
    void clearAttackers();
    int getCoins();
    int attackerID;
protected:
    int _score;    // score calculated at the end of the game. could be calculated in class CGame
    int _coins;    // current amount of money player has available to buy attackers
    Point _spawnLane;
    std::string attackerType;
    Map* _map = nullptr;
    defEntity _def; // definitions of towers loaded from file
    std::vector< std::unique_ptr<Attacker> > _attackers;
    std::deque<std::string> _attackersQueue;
};
