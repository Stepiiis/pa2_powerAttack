#pragma once

#include "Entity.h"
#include "constants.h"
#include "Map.h"
#include "Entity.h"
#include "exceptions.h"
#include "HelpStructs.h"
#include "Definitions.h"

#include <vector>
#include <set>
#include <memory>
#include <queue>
#include <deque>
#include <utility>


class Attacker: public Entity
{
public:
    /**
     * Constructor of abstract class Attacker - used to instantiate derived classes basicAttacker, fastAttacker, chargerAttacker
     * @param posX - initial x coordinate of attacker
     * @param posY - initial y coordinate of attacker
     * @param hp - hp at spawn
     * @param dmg - damage, that attacker can give
     * @param symbol - symbol which will be shown on map
     * @param radius - reach of attacker, where he can attack
     * @param attackSpeed - speed at which attacker can move (lower is faster)
     * @param map - pointer to a shared instance of Class Map
     * @param attackerID - id of current attacker
     */
    Attacker(int posX, int posY,int hp, int dmg, char symbol, int radius, int attackSpeed, Map * map,int attackerID);

    /** virtual destructor of abstract class */
    ~Attacker() override = default;

    /** operator < used for ordering inside a map, compares ids of Attackers @param rhs right hand side of comparison */
    bool operator < (Attacker & rhs);

    /** operator < used for ordering inside a map, compares ids of Attackers @param rhs right hand side of comparison */
    bool operator > (Attacker & rhs);

    /** virtual abstract method which checks wether a certain derived class can use the point passed as parameter
     * @param point point to be checked if derived attacker can use it to move
     * @returns false if attacker can not use it
     * @returns true if attacker can use it
     * */
    virtual bool checkSpecialization(const Point &point) = 0;

    /** moves attacker to first point in deque m_path and removes point from queue
     * @returns true if movement went correct
     * @returns false if attacker could not move (path empty, next in path is Exit/Attacker, updating of Map went wrong)*/
    bool moveOnPath();

    /** @returns first Point in m_path and doesn't remove it from deque, if its empty, it returns empty Point{x = -10, y = -10}*/
    Point getNextPoint();

    /** Absract virtual function which is used by BFS algrithm to determine end of search for each type of attacker
     * @returns true if point type of paramter p is the target of given Attacker. Otherwise returs false.
     * @param p - const reference to point on map whose current type we check.*/
    [[nodiscard]] virtual bool isTarget(const Point& p) const = 0;

    /** Used to polymorphically edit Map. @returns type of entity.*/
    Point::PointType getType() override;

    /** Used to save the game. @returns name of Attacker. */
    [[nodiscard]] std::string getTypeName() const override = 0;

    /** finds the shortest path to given target for given attacker using the BFS algortihm.
     * Uses polymorphic functions inside to determine end and usable points.
     * @returns true if path was found, otherwise returns false
     * @param endType default parameter that specifies to go to the Exit of Map.*/
    bool findShortestPath(Point::PointType endType = Point::Exit);

    /**@returns read write reference to Path dequeue */
    std::deque<Point>& getPath();

    /** adds effects to current entity. @param eff structure CEffects which specifies which effects to assign to attacker*/
    void addEffects(CEffects eff);

    /** @returns current effects that are on Attacker. */
    CEffects getEffects() const;

    /** @returns true if Attacker has a slow effect currently on. false otherwise. */
    [[nodiscard]] bool hasSlowEffect() const;

    /**@returns true if slower movement was assigned. false otherwise (wont happen). */
    bool setSlowerMovement();

    /** @returns true if default movement speed was set. false otherwise. */
    bool setNormalMovement();

    /** @returns which type is used by Attacker to hide from Towers */
    virtual Point::PointType pointToHide(); // not used anymore


protected:
    void popPath();
    std::deque<Point> m_path; // queue that is updated everytime findPath is called
    int m_cycleCnt{};
    int m_movementSpeed;
    CEffects m_effects{};
    int m_defaultMovementSpeed;
};

// symbol $
// Základní nejlevnější útočník. Běží směrem k cíli a pokud po cestě vidí ve svém radiusu věž, útočí na ni.
class basicAttacker final: public Attacker
{
public:
    basicAttacker(int posX, int posY, defEntity& def, Map* map, int id, int hp = -10);
    bool isTarget(const Point& p) const override;
    [[nodiscard]] std::string getTypeName() const override;
    bool checkSpecialization(const Point &point) override;
};


// Symbol %
// Rychlý útočník, který se vyhíbá slowEffectTowers a má velmi nízký damage, Útočí však 2x rychleji jak ostatní.
// Nízký počet HP, menší útok než basic attacker.
// velmi efektviní v kombinaci s chargerAttacker, který bude sám brát damage od tower, zatím co fast attacker může útočit
class fastAttacker final: public Attacker
{
public:
    fastAttacker(int posX, int posY, defEntity& def, Map* map, int id, int hp = -10);
    bool isTarget(const Point& p) const override;
    [[nodiscard]] std::string getTypeName() const override;
    bool checkSpecialization(const Point &point) override;
    Point::PointType pointToHide() override;
};

// symbol @
// Velmi vysoký počet HP, menší útok.
// Po vyslání si vybere nejbližší Vež, na kterou útočí dokud buď nezemře anebo věž nezničí. Pokud věž nezničí, tak pokračuje dál k další věži.
// Nejdražší z attackerů. Pravděpodobně 3x - 4x dražší než základní attacker.
class chargerAttacker final: public Attacker
{
public:
    chargerAttacker(int posX, int posY, defEntity& def, Map* map,int id, int hp = -10);
    bool isTarget(const Point& p) const override;
    [[nodiscard]] std::string getTypeName() const override;
    bool checkSpecialization(const Point &point) override;
};
