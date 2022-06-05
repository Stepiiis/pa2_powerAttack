#pragma once
#include "Entity.h"
#include "Attacker.h"
#include "exceptions.h"
#include <memory>


class Tower : public Entity {
public:
    /**
     * Constructor of abstract class Tower - used to instantiate derived classes
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
    Tower(int posX, int posY,int hp, int dmg, char symbol, int radius, int attackSpeed, Map * map,int attackerID);
    /** default virtual destructor for abstract class */
    ~Tower() override = default;
    /** operator < used for ordering inside a container, compares ids of Towers @param rhs right hand side of comparison */
    bool operator < (Tower & rhs);
    /** operator > used for ordering inside a container, compares ids of Towers @param rhs right hand side of comparison */
    bool operator > (Tower & rhs);
    /**
     * virtual method which returns effects which a derived Tower can give
     * @returns CEffect instance which specifies which effect are dealt by entity who calls this function
     */
    [[nodiscard]] virtual CEffects  getEffect() const =0; // returns effects which should be given on attack to enemy
    /// @returns type of Entity. Used to save into savefile.
    Point::PointType getType() override;
    /// @returns name of given Tower, used to save into savefile.
    [[nodiscard]] std::string getTypeName() const override = 0;
};


// základní věž se symbolem T
class basicTower final: public Tower{
public:
    /**
     * Constructor of basicTower gets reference to definitons and then passes correct values to the Tower constructor
     * @param x coordinate to be spawned on
     * @param y coordinate to be spawned on
     * @param def reference to definition of towers
     * @param map pointer to a shared map
     * @param towerID id of tower
     * @param hp optional argument if we want to override the hp specified by the definition file
     */
    basicTower(int x, int y, defEntity &def, Map* map,int towerID, int hp = -10);
    [[nodiscard]] std::string getTypeName() const override;
    [[nodiscard]] CEffects  getEffect() const override;
};


// Střílí dvě rány za jednu interakci
class fastTower final: public Tower{
public:
    fastTower(int x, int y, defEntity& def, Map* map,int towerID, int hp = -10);
    [[nodiscard]] std::string getTypeName() const override;
    [[nodiscard]] CEffects  getEffect() const override;
};

// útok od této věže je ~1,5x větší než základ. je však 2x dražší
class highDamageTower final: public Tower
    {
public:
    highDamageTower(int x, int y, defEntity &def, Map* map,int towerID, int hp = -10);
    [[nodiscard]] std::string getTypeName() const override;
    [[nodiscard]] CEffects  getEffect() const override;
    };


// útok od této věže způsobí to, že attacker po dobu 5 cyklů bude mít zpomalený pohyb.
class slowEffectTower final: public Tower
        {
public:
    slowEffectTower(int x, int y, defEntity& def, Map* map,int , int hp = -10);
    [[nodiscard]] std::string getTypeName() const override;
    [[nodiscard]] CEffects  getEffect() const override;
        };