#pragma once

#include "Entity.h"
#include "constants.h"
#include <vector>
#include <memory>
#include "Map.h"


class Attacker: public Entity
{
public:
    Attacker(int posX, int posY, int maxHealth, Map* map);
//    ~Attacker() = default;
    char getSymbol() override;
    ~Attacker() override;


    bool checkRadius() override;
    void setPosition(int x, int y);
    void place(int x, int y);


};

// symbol $
// Základní nejlevnější útočník. Běží směrem k cíli a pokud po cestě vidí ve svém radiusu věž, útočí na ni.
class basicAttacker final: public Attacker
{
public:
    basicAttacker(int posX, int posY, int maxHealth, Map* map);
    char getSymbol() override;
    bool checkRadius() override;
};


// Symbol %
// Rychlý útočník, který se vyhíbá slowEffectTowers a má velmi nízký damage, Útočí však 2x rychleji jak ostatní.
// Nízký počet HP, menší útok než basic attacker.
// velmi efektviní v kombinaci s chargerAttacker, který bude sám brát damage od tower, zatím co fast attacker může útočit
class FastAttacker final: public Attacker
{
public:
    FastAttacker(int posX, int posY, int maxHealth, Map* map);
    char getSymbol() override;
    bool checkRadius() override;
};

// symbol @
// Velmi vysoký počet HP, menší útok.
// Po vyslání si vybere nejbližší Vež, na kterou útočí dokud buď nezemře anebo věž nezničí. Pokud věž nezničí, tak pokračuje dál k další věži.
// Nejdražší z attackerů. Pravděpodobně 3x - 4x dražší než základní attacker.
class chargerAttacker final: public Attacker
{
public:
    chargerAttacker(int posX, int posY, int maxHealth, Map* map);
    char getSymbol() override;
    bool checkRadius() override;

};
