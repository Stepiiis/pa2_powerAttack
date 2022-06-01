
#include "Enemy.h"


Enemy::Enemy(Map * map,defEntity def, int dif)
: _def(std::move(def)), _difficulty(dif)
{
    _map = map;
    _towerIDcnt = 0;
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
        for (int i = 0; i < nrTowers; i++) {
            int x = (int)rng() % MAP_WIDTH;
            int y = (int)rng() % MAP_HEIGHT;
            if(x<0 || y<0) {i--; continue;}
            if (_map->m_map[y][x]._type == Point::Empty) {
                if (x <= MAP_WIDTH - 1 && x >= 0 && y <= MAP_HEIGHT - 1 && y >= 0) {
                    if(_map->checkNeighbours(x,y)){
                        _towers.emplace_back(new basicTower(x, y, _def["basicTower"]["hp"], _map, _towerIDcnt++));
                        _emptySpaces[y][x] = 'T';
                        _map->updateCell(x,y,Point::Tower,(char)_def["basicTower"]["symbol"]);
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
size_t Enemy::getTowerCount(){
    return _towers.size();
}
