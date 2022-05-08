
#include "Enemy.h"
#include "Map.h"
#include "constants.h"
#include <random>

Enemy::Enemy(Map * map,towerDefs def, int dif)
: _def(def), _difficulty(dif)
{
    _map = map;
}

Enemy::~Enemy() = default;


void Enemy::findEmptySpaces(){
    _map->getEmptySpaces(_emptySpaces);
}

void Enemy::clearTowers(){
    for(auto& tower : _towers){
        tower->destroy();
    }
    _towers.clear();
}
// randomly creates towers throughout the map and adds them to the vector of towers with their respective coordinates
void Enemy::createTowers(){
    int nrTowers = _difficulty * 3;
    std::random_device rds;
    std::mt19937 rng(rds());
    if(_difficulty == 1) {
        // FIXME: TOWERS are sometimes created but not showed on the map
        for (int i = 0; i < nrTowers; i++) {
            int x = rng() % MAP_WIDTH;
            int y = rng() % MAP_HEIGHT;
            if (_map->m_map[y][x].type == Point::Empty) {
                if (x <= MAP_WIDTH - 1 && x >= 0 && y <= MAP_HEIGHT - 1 && y >= 0) {
                    if(_map->checkNeighbours(x,y)){
                        _towers.emplace_back(new basicTower(x, y, _def.base.maxHP, _map));
                        _emptySpaces[y][x] = 'T';
                    }else{
                        i--;
                        continue;
                    }
                }
            } else {
                i--;
            }
        }
    }
    else if(_difficulty == 2) {
        // TODO
    }
    else if(_difficulty == 3) {
        // TODO
    }
}

// goes trough all the towers and draws them onto the map if their HP is bigger than 0
void Enemy::printTowers(){
    for(auto& tower : _towers){
        if(tower->getHP() > 0){
            tower->draw();
        }
    }
//    getch();
}
int Enemy::getTowerCount(){
    return _towers.size();
}
