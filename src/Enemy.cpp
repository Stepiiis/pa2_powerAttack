
#include "Enemy.h"
#include "Map.h"
#include "constants.h"
#include <random>

Enemy::Enemy(Map * map,int towerHp, int range, int damage, int dif)
: Player(map,towerHp, range, damage), _difficulty(dif)
{
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
            int x = rng() % MAP_WIDTH;       // FIX ME random not working.
            int y = rng() % MAP_HEIGHT;       // sometimes creates towers in the border of the map
            if (_map->m_map[y][x].type == Point::Empty) {
                if (x <= MAP_WIDTH - 1 && x >= 0 && y <= MAP_HEIGHT - 1 && y >= 0) {
                    if(_map->checkNeighbours(x,y)) {
                        _towers.emplace_back(new Tower(x, y, _baseHp, _map));
                        _emptySpaces[y][x] = 'T';
                    }
                }
            } else {
                i--;
            }
        }
    }
//    }else if(_difficulty == 2){
//        for (int i = 0; i < nrTowers; i++) {
//            int x = rand() % _emptySpaces.size();
//            int y = rand() % _emptySpaces.size();
//            if(_emptySpaces[x][y] == ' '){
//                if(i % 3 == 0){
//                    _towers.emplace_back(new fastTower(x, y, _baseHp, _map));
//                    _emptySpaces[x][y] = 'T';
//                }else {
//                    _towers.emplace_back(new Tower(x, y, _baseHp * 2, _map));
//                    _emptySpaces[x][y] = 'T';
//                }
//            }
//            else{
//                i--;
//            }
//        }
//    }
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