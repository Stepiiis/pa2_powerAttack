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
    Attacker(int posX, int posY,int hp, int dmg, char symbol, int radius, int attackSpeed, Map * map,int attackerID);
    ~Attacker() override = default;
    bool operator < (Attacker & rhs);
    bool operator > (Attacker & rhs);

    virtual bool checkSpecialization(const Point &point) = 0;

    bool moveOnPath(); // moves attacker to next point
    Point getNextPoint(); // returns next point on path
    [[nodiscard]] virtual bool isTarget(const Point& p) const = 0; // absract function for all derivates which returns target type used by BFS
    Point::PointType getType() override; // returns type of entity
    [[nodiscard]] std::string getTypeName() const override = 0; // returns name of entity
    bool findShortestPath(Point::PointType endType = Point::Exit); // finds shortest path using BFS
    std::deque<Point>& getPath();
    void addEffects(const CEffects eff);
    CEffects getEffects() const;
    [[nodiscard]] bool hasSlowEffect() const;
    bool setSlowerMovement();
    bool setNormalMovement();
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
