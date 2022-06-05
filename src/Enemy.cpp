

#include "Enemy.h"


Enemy::Enemy(Map * map,defEntity def, int dif)
: m_map(map), m_difficulty(dif), m_def(std::move(def))
{
    m_towerIDcnt = 0;

}

Enemy::~Enemy() = default;


void Enemy::clearTowers(){
    for(auto& tower : m_towers){
        tower.second->destroy();
    }
    m_towers.clear();
}
// randomly creates towers throughout the map and adds them to the vector of towers with their respective coordinates
void Enemy::createTowers(){
    int nrTowers = (m_difficulty+1) * 3;
    std::random_device rds;
    std::mt19937 rng(rds());
    if(m_difficulty == 0) {
        nrTowers += 2;
        for (int i = 0; i < nrTowers; i++) {
            int x = (int)rng() % MAP_WIDTH;
            int y = (int)rng() % MAP_HEIGHT;
            if(x<0 || y<0) {i--; continue;}
            if (m_map->m_map[y][x]._type == Point::Empty) {
                if (x <= MAP_WIDTH - 1 && x >= 0 && y <= MAP_HEIGHT - 1 && y >= 0) {
                    if(m_map->checkNeighbours(x, y)){
                        m_towers.emplace(m_towerIDcnt, new basicTower(x, y, m_def, m_map, m_towerIDcnt));
                        m_map->setEntity(x, y, m_towers[m_towerIDcnt].get());
                        m_towerIDcnt++;
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
    else if(m_difficulty == 1||m_difficulty == 2){
        Point konec = m_map->getExit();
        auto* testPathCreator =  new basicAttacker(konec._x,konec._y,m_def,m_map,0);
        testPathCreator->findShortestPath(Point::Entry);
        auto& path = testPathCreator->getPath();
        std::vector<Point> spawnPoints;
        for(int i{};i<nrTowers; ++i){
            int prvek = (int)(rng() % path.size());
            if(prvek<0 || prvek>=path.size()) {i--; continue;}
            auto p = path[prvek];
            int y = (int)rng() % 3;
            p._y += + y;
            if(!m_map->checkBounds(p._x, p._y)){i--; continue;}
            auto type = m_map->getType(p);
            if( type != Point::Empty){i--; continue;}
            int typeEnt = (int)(rng() % 4);
            if(typeEnt == 0){
                m_towers.emplace(m_towerIDcnt, new basicTower(p._x, p._y, m_def, m_map, m_towerIDcnt));

            }
            else if(typeEnt == 1){
                m_towers.emplace(m_towerIDcnt, new fastTower(p._x, p._y, m_def, m_map, m_towerIDcnt));

            }
            else if(typeEnt == 2){
                m_towers.emplace(m_towerIDcnt, new slowEffectTower(p._x, p._y, m_def, m_map, m_towerIDcnt));

            }
            else if(typeEnt == 3){
                m_towers.emplace(m_towerIDcnt, new highDamageTower(p._x, p._y, m_def, m_map, m_towerIDcnt));
            }
            m_map->setEntity(p._x, p._y, m_towers[m_towerIDcnt].get());
            m_towerIDcnt++;
        }
        delete testPathCreator;
    }
}

// goes trough all the towers and draws them onto the map if their HP is bigger than 0
void Enemy::printTowers(){
    for(auto& tower : m_towers){
        if(tower.second->getHP() > 0){
            tower.second->draw();
        }
    }
//    getch();
}
size_t Enemy::getTowerCount(){
    return m_towers.size();
}

int Enemy::damageTowers(std::vector<std::pair<int, int>> &towers) {

    std::unordered_set<int> toRemove;
    for(const auto & tower: towers){
        if(m_towers.count(tower.first)) {
            if (!m_towers[tower.first]->takeDamage(tower.second))
                toRemove.emplace(tower.first);
        }
    }
    for(int index : toRemove){
        if(m_towers.count(index)) {
            m_towers[index]->destroy();
            m_towers.erase(index);
        }
    }
    return (int)toRemove.size();
}

bool Enemy::createNewTower(int type, int x, int y, int hp) {

    switch (type){
        case 0:
            m_towers.emplace(m_towerIDcnt, std::make_unique<basicTower>(x, y, m_def, m_map, m_towerIDcnt, hp));
            break;
        case 1:
            m_towers.emplace(m_towerIDcnt, std::make_unique<fastTower>(x, y, m_def, m_map, m_towerIDcnt, hp));
            break;
        case 2:
            m_towers.emplace(m_towerIDcnt, std::make_unique<highDamageTower>(x, y, m_def, m_map, m_towerIDcnt, hp));
            break;
        case 3:
            m_towers.emplace(m_towerIDcnt, std::make_unique<slowEffectTower>(x, y, m_def, m_map, m_towerIDcnt, hp));
            break;
        default:
        throw std::invalid_argument("type doesnt correspond any tower definition");
    }
    m_towerIDcnt++;
    return true;
}

std::vector<std::tuple<int, int, CEffects> > Enemy::getAttackersToAttack() {
    std::vector<std::tuple<int, int,CEffects> > attackers;
    for (auto &attacker: m_towers) {
        if(attacker.second->checkRadius(Point::Attacker))
            if(attacker.second->canAttack())
               attackers.emplace_back(attacker.second->getCurrentFocus(), attacker.second->getDamage(), attacker.second->getEffect());
    }
    return attackers;
}

const std::map<int, std::unique_ptr<Tower> > &Enemy::getTowers() const {
    return m_towers;
}
