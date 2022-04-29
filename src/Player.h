#pragma once
#include "Entity.h"
#include <memory>
class Player : public Entity {
public:
    Player();
    Player(const Player&);
    Player& operator=(const Player&);
    ~Player();
private:
    int m_score;
    int m_currency;
    vector<unique_ptr<Entity >>;
};
