#include "Player.h"
Player::Player(Map* map,defEntity def)
{
    _def = def;
    _map = map;
    _spawnLane.x=-10;
    _spawnLane.y=-10;
    attackerID = 0;
}

void Player::setLane(int lane) {
    if (!_map->getLaneByID(lane, _spawnLane)){
        _spawnLane.x = -10;
        _spawnLane.y = -10;
    }
    _map->highlightLane(lane);
}
void Player::setCoins(int coins){
    _coins = coins;
}
int Player::getCoins(){
    return _coins;  
}

  bool Player::spawnAttacker(){
    if(_spawnLane.x==-10) // TODO: "callback" that player didnt choose lane
        return false;
     if(_def[_attackersQueue.front()]["price"]>_coins) // TODO: "CALLBACK" not enough money
         return false;
    _coins-=_def[_attackersQueue.front()]["price"];
    attackerID++;
    switch (_def[_attackersQueue.front()]["symbol"]){ // chooses on the basis of different ascii values
        case 36: // ascii value of $
            _attackers.insert(std::upper_bound(_attackers.begin(),_attackers.end(),std::make_unique<basicAttacker>(_spawnLane.x,_spawnLane.y,_def[BASICA]["hp"],_map,attackerID)),std::make_unique<basicAttacker>(_spawnLane.x,_spawnLane.y,_def[BASICA]["hp"],_map,attackerID));
            break;
        case 37: // %
            _attackers.insert(std::upper_bound(_attackers.begin(),_attackers.end(),std::make_unique<fastAttacker>(_spawnLane.x,_spawnLane.y,_def[BASICA]["hp"],_map,attackerID)),std::make_unique<fastAttacker>(_spawnLane.x,_spawnLane.y,_def[BASICA]["hp"],_map,attackerID));
            break;
        case 64: // @
            _attackers.insert(std::upper_bound(_attackers.begin(),_attackers.end(),std::make_unique<chargerAttacker>(_spawnLane.x,_spawnLane.y,_def[BASICA]["hp"],_map,attackerID)),std::make_unique<chargerAttacker>(_spawnLane.x,_spawnLane.y,_def[BASICA]["hp"],_map,attackerID));
            break;
    }
    _attackersQueue.pop();
    return true;
}

void Player::setAttackerType(int type) {
    switch (type)
    {
        case 0:
            attackerType = BASICA ;
            break;
        case 1:
            attackerType = FASTA;
            break;
        case 2:
            attackerType = CHARGERA;
            break;
    }
}

void Player::addAttackerToQueue() {
    _attackersQueue.push(attackerType);
}

void Player::moveAttackers() {
    for (auto &attacker: _attackers) {
        if (attacker->findShortestPath()) {
            attacker->moveOnPath();
        }
    }
}

bool Player::emptyAttackers() {
    return _attackers.empty();
}

bool Player::emptyAttackerQueue() {
    return _attackersQueue.empty();
}
