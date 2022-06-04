#include "Player.h"

#include <utility>
Player::Player(Map* map,defEntity def)
{
    _def = std::move(def);
    _map = map;
    _spawnLane._x=-10;
    _spawnLane._y=-10;
}


void Player::setLane(int lane) {
    if (!_map->getLaneByID(lane, _spawnLane)){
        _spawnLane._x = -10;
        _spawnLane._y = -10;
    }
    _map->highlightLane(lane);
}
void Player::setCoins(int coins){
    _coins = coins;
}
int Player::getCoins() const{
    return _coins;  
}



  bool Player::spawnAttacker(){
      auto attacker = _attackersQueue.front();
    if(attacker.second._x == -10) // TODO: "callback" that player didnt choose lane
        return false;
    if(_def[attacker.first]["price"]>_coins) // TODO: "CALLBACK" not enough money
        return false;
    _attackersQueue.pop_front();
    _coins-=_def[attacker.first]["price"];
    switch (_def[attacker.first]["symbol"]){ // chooses on the basis of different ascii values
        case 36: // ascii value of $
            _attackers.emplace(attackerID,std::make_unique<basicAttacker>(attacker.second._x, attacker.second._y, _def, _map, attackerID));
            break;
        case 37: // %
            _attackers.emplace(attackerID,std::make_unique<fastAttacker>(attacker.second._x, attacker.second._y, _def, _map, attackerID));
            break;
        case 64: // @
            _attackers.emplace(attackerID,std::make_unique<chargerAttacker>(attacker.second._x, attacker.second._y, _def, _map, attackerID));
            break;
    }
      attackerID++;
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
        default:
            attackerType = BASICA; // redundant
            break;
    }
}

void Player::addAttackerToQueue() {
    _attackersQueue.emplace_back(attackerType, _spawnLane);
}

void Player::moveAttackers() {
    std::vector<int> toDelete;
    for (auto &attacker: _attackers) {

        if (attacker.second->findShortestPath()) {
            if(!attacker.second->moveOnPath()) {
                if(attacker.second->getNextPoint()._type == Point::Exit){
                    _attackersFinished++;
                    toDelete.push_back(attacker.first);
                    attacker.second->destroy();
                }
            }
        }
    }
    for (auto &id: toDelete) {
        _attackers.erase(id);
    }
}

bool Player::emptyAttackers() {
    return _attackers.empty();
}

bool Player::emptyAttackerQueue() {
    return _attackersQueue.empty();
}

void Player::clearAttackers() {
    for (auto &attacker: _attackers) {
        attacker.second->destroy();
    }
    _attackers.clear();
    _attackersQueue.clear();
}

int Player::getFinished() const {
    return _attackersFinished;
}

auto& Player::getAttackers() const {
    return _attackers;
}

std::vector<std::pair<int, int> > Player::getTowersToAttack() const {
    std::vector<std::pair<int, int> > towers;
    for (auto &attacker: _attackers) {
        if(attacker.second->checkRadius(Point::Tower))
            if(attacker.second->isFocused())
                towers.emplace_back(attacker.second->getCurrentFocus(), attacker.second->getDamage());
    }
    return towers;
}

bool Player::createNewAttacker(int type, int x, int y, int hp, CEffects effects, int id) {
    switch (type){
        case 0:
            _attackers.emplace(id, std::make_unique<basicAttacker>(x,y,_def,_map,id,hp));
            break;
        case 1:
            _attackers.emplace(id, std::make_unique<fastAttacker>(x,y,_def,_map,id,hp));
            break;
        case 2:
            _attackers.emplace(id, std::make_unique<chargerAttacker>(x,y,_def,_map,id,hp));
            break;
    }
    return true;
}

bool Player::printAttackers() {
    for(const auto& attacker: _attackers){
        if(attacker.second->getHP()>0)
            attacker.second->draw();
    }
    return true;
}

void Player::setFinished(int nr) {
    _attackersFinished = nr;
}

void Player::addAttackersToQueue(std::deque<std::pair<std::string, Point> >& queue) {
    _attackersQueue = queue;
}

size_t Player::getAttackerCount() {
    return _attackers.size();
}

int Player::damageAttackers(std::vector<std::pair<int,int> >& attackers) {
    std::vector<int> toRemove;
    for(const auto & attacker: attackers){
        if(_attackers.count(attacker.first)) {
            if (!_attackers[attacker.first]->takeDamage(attacker.second))
                toRemove.emplace_back(attacker.first);
        }
    }
    for(int index : toRemove){
        if(_attackers.count(index)) {
            _attackers[index]->destroy();
            _attackers.erase(index);
        }
    }

    return toRemove.size();
}

