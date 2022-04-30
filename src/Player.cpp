#include "Player.h"
Player::Player(Map& map,int baseHp, int range, int damage)
        : _baseHp(baseHp), _baseRange(range), _baseAtk(damage)
{
    _map = std::make_shared<Map>(map);
}