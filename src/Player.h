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
    /**
     * Constructor of Player
     * @param map address of map which was instantiated by Game
     * @param def definitions of Attackers
     * */
    Player(Map* map, defEntity def);
    Player(const Player&) = default;
    Player& operator=(const Player&);
    ~Player() = default;

    /**
     * sets current spawnLane to lane specified in parameter
     * @param lane lane to be set as spawnLane
     * if the lane parameter is not within bounds ->spawnLane is set as Point{x = -10,y = -10}
     */
    void setLane(int lane);

    /**
     * sets current attacker to be spawned as the one specified in parameter type
     * @param type specifies which atacker to be selected (0 - BASIC, 1 - CHARGER, 2 - FAST)
     */
    void setAttackerType(int type);

    /**
     * upon pressing spacebar the attacker type currently chosen is added to queue of attackers to be spawned
     * @returns true if player had enough money to buy this attacker, otherwise false;
     */
    bool addAttackerToQueue();

    /**
     * Takes an attacker from queue and spawns it onto the screen if a path to given destination is available.
     * If its not possible to find a path at at that moment, it will return a string specifying the problem, which can be preinted on screen.
     * @returns string of errorValues. If the string is not empty, it will be printed on screen.
     * */
    std::string spawnAttacker();

    /**
     * Goes trough all the attackers on screen and if their hp is bigger that 0 they are drawn on screen.
     * */
    void printAttackers();

    /**
     * creates new attacker according to values specified in parrameters
     * @param type specifies which attacker to spawn (0 - BASIC, 1 - FAST, 2 - CHARGER)
     * */
    bool createNewAttacker(int type, int x, int y, int hp, CEffects effects); //type  0 = basic, 1 = fast, 2 = charger
    /**
     * Moves all attackers according to their current path. If the next position is occupied, it stays on the current position.
     * */
    void moveAttackers();

    /**
     * checks wether the container of attackers is empty or not
     * @returns true if they are empty and false if they are not
     * */
    bool emptyAttackers();

    /**
     * checks wether the queue of attackers is empty.
     * @returns true if attackersQueue is empty. false if not.
     * */
    bool emptyAttackerQueue();

    /**
     * sets current Attackers queue to one passed as argument
     * @param queue specifies the new content of queue to which current one should be overwritten.
     * */
    void setAttackersQueue(std::deque<std::pair<std::string, Point> >& queue);

    /**
     * returns queue of attackers in queue. used by save function
     * @returns read only reference to attackers queue
     * */
    const std::deque<std::pair<std::string, Point>> & getAttackersQueue();
    /** setter for m_coins value */
    void setCoins(int coins);

    /**
     * clears container of attackers
     * */
    void clearAttackers();
    /**
     * damages all attackers specified by the first value in tupple pased as parameter
     * @param attackers vector of tupples which should be attacked
     * @param attackers get\<0> - ID of attacker\n get\<1> - amount of damage to be given to the attacker specified by ID\n get\<2> - effects to be given to the attacker specified by ID
     * */
    int damageAttackers(std::vector<std::tuple<int,int,CEffects> > & attackers);

    /**
     * sets amount of attackers who finished to one specified in parameter
     * @param nr value to be set as number of attackers finished
     * */
    void setFinished(int nr);
    /**
     * @returns number of attackers which have already went through the exit
     * */
    int getFinished() const;

    /**
     * @returns amount of coins currently available
     * */
    int getCoins() const;
    /**
     * @returns the amount of attackers in container.
     * */
    size_t getAttackerCount();

    /**
     * @returns Vector of pairs specifiying which Towers were in the perimeter of given attacker and are to be attacked\n
     * get\<0> - ID of tower to be dealt damage\n get\<1> - amount of damage to be given\n
     * */
    [[nodiscard]] std::vector<std::pair<int,int> > getTowersToAttack() const;

    /** @returns read only reference to container of Attackers */
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
    std::map<int, std::unique_ptr<Attacker> > m_attackers; // container of attackers in game
    std::deque<std::pair<std::string,Point>> m_attackersQueue; // container of attackers to be spawned
};
