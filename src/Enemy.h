#pragma once
#include "Player.h"
#include "Tower.h"
#include "Map.h"
#include "Definitions.h"
#include "logFile.h"
#include "Map.h"
#include "constants.h"
#include <memory>
#include <vector>
#include <ncurses.h>
#include <random>
#include <unordered_set>

// handler of building and attacking of towers
class Enemy{
public:
    Enemy(Map* map,defEntity def, int dif);
    ~Enemy();
    void clearTowers(); // erases old towers from the map
    void findEmptySpaces(); // finds empty spaces where towers can be built
    void createTowers(); // creates towers according to the difficulty chosen
    void printTowers(); // prints all created towers onto the map
    size_t getTowerCount(); // returns the number of towers
    int damageTowers(std::vector<std::pair<int, int> > &towers); // damages all towers in the vector
    bool createNewTower(int type, int x, int y, int hp); // 0 = BASIC, 1 = FAST, 2 = STRONG, 3 = SLOWEF
    std::vector<std::tuple<int,int, CEffects> > getAttackersToAttack(); // returns container of attackers which are to be damaged
    [[nodiscard]] const std::map<int, std::unique_ptr<Tower> >& getTowers() const; // returns the container of towers
protected:
    Map* m_map; // pointer to shared map
    std::map<int,std::unique_ptr<Tower> > m_towers; // map of towers
    int m_towerIDcnt;// used as an auto incrementing index for the towers
    int m_difficulty;
    defEntity m_def; // definitions of towers loaded from file
    std::vector<std::vector<char> > m_emptySpaces;
};