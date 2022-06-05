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

    /** Instanciates an entity and sets default parameters*/
    Enemy(Map* map,defEntity def, int dif);

    /** Default destructor because structure doesnt use any manual heap allocations*/
    ~Enemy() = default;

    /** Erases all towers from Map and deletes them from the structure m_towers.*/
    void clearTowers();

    /**Creates all towers according to current difficulty set during constructing of Enemy.
     * Doesnt input them into the Map structure and doesnt print them. printTowers() has to be called to facilitiate that.
     * Easiest (default value if not changed in options menu) difficulty creates 5 towers randomly on Map.
     * Medium and Hard uses BFS to detertmine a path likely to be used by attackers and places Towers around it.
     * Medium spawns 6 Towers and Hard 9 Towers.
     * */
    void createTowers();

    /** Prints all towers created onto the screen.*/
    void printTowers();

    /** @returns number of towers currently alive.*/
    size_t getTowerCount(); // returns the number of towers

    /** finds all Towers according to their respective IDs and damages them accordingly.
     * @returns number of Towers that doied during interaction
     * @param towers.first reference to id of tower to be damaged
     * @param towers.second reference to the amount of damage to be given to the Tower.
     * */
    int damageTowers(std::vector<std::pair<int, int> > &towers);
    /** creates new Tower of certain type specified in parameters
     * @param type specifies which tower to instantiate - 0 = BASIC, 1 = FAST, 2 = STRONG, 3 = SLOWEF
     * @param x x coordinate where to spawn
     * @param y y coordinate where to spawn
     * @param hp amount of HP on spawn
     * */
    bool createNewTower(int type, int x, int y, int hp);

    /** Checks perimeter around all Towers and saves into a vector of tuples which it returns. @returns a vector of tuples which specifies who to attack with how much damage and with what efects
     * get<0> - ID of attacker, get<1> - amount of damage to be given, get<2> - Effects to be given to attacker.*/
    std::vector<std::tuple<int,int, CEffects> > getAttackersToAttack();

    /** @returns a read only reference to container of towers currently instantiated. */
    [[nodiscard]] const std::map<int, std::unique_ptr<Tower> >& getTowers() const; // returns the container of towers
protected:
    Map* m_map; // pointer to shared map
    std::map<int,std::unique_ptr<Tower> > m_towers; // map of towers
    int m_towerIDcnt;// used as an auto incrementing index for the towers
    int m_difficulty;
    defEntity m_def; // definitions of towers loaded from file
    std::vector<std::vector<char> > m_emptySpaces;
};