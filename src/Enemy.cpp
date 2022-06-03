
#include "Enemy.h"


Enemy::Enemy(Map * map,defEntity def, int dif)
:  _map(map),_difficulty(dif),_def(std::move(def))
{
    _towerIDcnt = 0;

}

Enemy::~Enemy() = default;


void Enemy::findEmptySpaces(){
    _map->getEmptySpaces(_emptySpaces);
}

void Enemy::clearTowers(){
    for(auto& tower : _towers){
        tower.second->destroy();
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
                        _towers.emplace(_towerIDcnt,new basicTower(x, y, _def, _map, _towerIDcnt));
                        _emptySpaces[y][x] = 'T';
                        _map->setEntity(x,y,_towers[_towerIDcnt].get());
                        _towerIDcnt++;
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
        if(tower.second->getHP() > 0){
            tower.second->draw();
        }
    }
//    getch();
}
size_t Enemy::getTowerCount(){
    return _towers.size();
}

bool Enemy::damageTowers(std::vector<std::pair<int, int>> &towers) {

    std::vector<int> toRemove;
    for(const auto & tower: towers){
        if(_towers.count(tower.first)) {
            if (!_towers[tower.first]->takeDamage(tower.second))
                toRemove.emplace_back(tower.first);
        }
    }
    for(int index : toRemove){
        if(_towers.count(index)) {
            _towers[index]->destroy();
            _towers.erase(index);
        }
    }
    return true;
}

bool Enemy::createNewTower(int type, int x, int y, int hp, int id) {
    switch (type){
        case 0:
            _towers.emplace(id, std::make_unique<basicTower>(x,y,_def,_map,id,hp));
            break;
        case 1:
            _towers.emplace(id, std::make_unique<fastTower>(x,y,_def,_map,id,hp));
            break;
        case 2:
            _towers.emplace(id, std::make_unique<highDamageTower>(x,y,_def,_map,id,hp));
            break;
        case 3:
            _towers.emplace(id, std::make_unique<slowEffectTower>(x,y,_def,_map,id,hp));
            break;
        default:
        throw std::invalid_argument("type doesnt correspond any tower definition");
    }
    return true;
}
