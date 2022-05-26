#include "Player.h"
Player::Player(Map* map,defEntity def)
{
    _def = def;
    _map = map;
    _spawnLane.x=-10;
    _spawnLane.y=-10;
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

 [[nodiscard]] bool Player::spawnAttacker(int type){
    if(_spawnLane.x==-10) // TODO: "callback" that player didnt choose lane
        return false;
    switch (type){
        case 0:
            if(_def[BASICA]["price"]>_coins)
                return false;
            _attackers.push_back(new basicAttacker(_spawnLane.x,_spawnLane.y,_def[BASICA]["hp"],_map));
            break;
        case 1:
            if(_def[FASTA]["price"]>_coins)
                return false;
            _attackers.push_back(new fastAttacker(_spawnLane.x,_spawnLane.y,_def[FASTA]["hp"],_map));
            break;
        case 2:
            if(_def[CHARGERA]["price"]>_coins)
                return false;
            _attackers.push_back(new chargerAttacker(_spawnLane.x,_spawnLane.y,_def[CHARGERA]["hp"],_map));
            break;
    }
}