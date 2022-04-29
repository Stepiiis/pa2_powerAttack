
#include "Enemy.h"


Enemy::Enemy(std::shared_ptr<Map> map,int towerHp, int range, int damage, int dif, int level)
: Player(map,std::move(towerHp), std::move(range), std::move(damage)), _difficulty(dif), _level(level)
{
}
void Enemy::findEmptySpaces(Map& currMap){
    _emptySpaces = std::move(currMap.getEmptySpaces());
}

void Enemy::clearTowers(){
    for(auto& tower : _towers){
        tower->destroy();
    }
}

void Enemy::createTowers(){
    int nrTowers = _difficulty * _level * 3;
    if(_difficulty == 1) {
        for (int i = 0; i < nrTowers; i++) {
            int x = rand() % _emptySpaces.size();
            int y = rand() % _emptySpaces.size();
            if(_emptySpaces[x][y] == ' '){
                _towers.emplace_back(new Tower(x, y, _baseHp, _map));
                _emptySpaces[x][y] = 'T';
            }
            else{
                i--;
            }
        }
    }else if(_difficulty == 2){
        for (int i = 0; i < nrTowers; i++) {
            int x = rand() % _emptySpaces.size();
            int y = rand() % _emptySpaces.size();
            if(_emptySpaces[x][y] == ' '){
                if(i % 3 == 0){
                    _towers.emplace_back(new fastTower(x, y, _baseHp, _map));
                    _emptySpaces[x][y] = 'T';
                }else {
                    _towers.emplace_back(new Tower(x, y, _baseHp * 2, _map));
                    _emptySpaces[x][y] = 'T';
                }
            }
            else{
                i--;
            }
        }
    }
}