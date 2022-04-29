#include "Player.h"
Player::Player(std::shared_ptr<Map> map,int baseHp, int range, int damage)
        : _map(map),_baseHp(baseHp), _baseRange(range), _baseAtk(damage)
{
}