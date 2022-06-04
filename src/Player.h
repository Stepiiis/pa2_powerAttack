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
    bool addAttackerToQueue();
    bool spawnAttacker();
    bool printAttackers();
    bool createNewAttacker(int type, int x, int y, int hp, CEffects effects); //type  0 = basic, 1 = fast, 2 = charger
    void moveAttackers();
    bool emptyAttackers();
    bool emptyAttackerQueue();
    void setAttackersQueue(std::deque<std::pair<std::string, Point> >& queue);
    std::deque<std::pair<std::string, Point> >& getAttackersQueue();
    void setCoins(int coins);
    void clearAttackers();
    int damageAttackers(std::vector<std::pair<int,int> > & attackers);
    void setFinished(int nr);
    int getFinished() const;
    int getCoins() const;
    size_t getAttackerCount();

    [[nodiscard]] std::vector<std::pair<int,int> > getTowersToAttack() const;
    [[nodiscard]] const std::map<int, std::unique_ptr<Attacker> >& getAttackers() const;
    int attackerID = 0;
protected:
    int _score{};    // score calculated at the end of the game. could be calculated in class CGame
    int _attackersFinished{};
    int _coins{};    // current amount of money player has available to buy attackers
    Point _spawnLane{};
    std::string attackerType;
    Map* _map = nullptr;
    defEntity _def; // definitions of towers loaded from file
    std::map<int, std::unique_ptr<Attacker> > _attackers;
    std::deque<std::pair<std::string,Point>> _attackersQueue;
};
