#include "Player.h"
Player::Player(Map* map,defEntity def)
{
    _def = def;
    _map = map;
}

void Player::setLane(int lane){
    _spawnLane= _map->getLaneByID(lane);
    _map->highlightLane(lane);
}
void Player::setCoins(int coins){
    _coins = coins;
}
int Player::getCoins(){
    return _coins;
}
void getCoins(int coins);