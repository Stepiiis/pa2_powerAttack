#pragma once
#include "Entity.h"
#include "Attacker.h"
#include "Definitions.h"
#include "Map.h"
#include <memory>
#include <vector>
#include <algorithm>
#include <utility>
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
    std::string spawnAttacker();
    bool printAttackers();
    bool createNewAttacker(int type, int x, int y, int hp, CEffects effects); //type  0 = basic, 1 = fast, 2 = charger
    void moveAttackers();
    bool emptyAttackers();
    bool emptyAttackerQueue();
    void setAttackersQueue(std::deque<std::pair<std::string, Point> >& queue);
    std::deque<std::pair<std::string, Point> >& getAttackersQueue();
    void setCoins(int coins);
    void clearAttackers();
    int damageAttackers(std::vector<std::tuple<int,int,CEffects> > & attackers);
    void setFinished(int nr);
    int getFinished() const;
    int getCoins() const;
    size_t getAttackerCount();

    [[nodiscard]] std::vector<std::pair<int,int> > getTowersToAttack() const;
    [[nodiscard]] const std::map<int, std::unique_ptr<Attacker> >& getAttackers() const;
protected:
    int m_attackerID;
    int m_score{};    // score calculated at the end of the game. could be calculated in class CGame
    int m_attackersFinished{};
    int m_coins{};    // current amount of money player has available to buy attackers
    Point m_spawnLane{};
    std::string m_attackerType;
    Map* m_map = nullptr;
    defEntity m_def; // definitions of towers loaded from file
    std::map<int, std::unique_ptr<Attacker> > m_attackers;
    std::deque<std::pair<std::string,Point>> m_attackersQueue;
};
