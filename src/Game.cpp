#include <cstring>
#include "Game.h"


CGame::~CGame() {
    delete _player;
    delete _tower_manager;
    delwin(_game_window); // zavreni okna s hrou
    endwin(); // dealoc pameti a zavreni ncurses
}

bool CGame::start() {
    _gameMap.printMap();
    drawAttackerDefs();
    if (resume())
        return true;
    return false;
}

bool CGame::initializeWindow() {
    refresh();
    keypad(_game_window, TRUE);
    box(_game_window, 0, 0);
    wrefresh(_game_window);
    return true;
}

// prevzato ze StackOverflow
// https://stackoverflow.com/questions/421860/capture-characters-from-standard-input-without-waiting-for-enter-to-be-pressed/67363091#67363091
int kbhit() {
    static int nbbytes;
    ioctl(0, FIONREAD, &nbbytes);
    return nbbytes;
}
// konec prevzateho

bool CGame::resume() {
    int input;
    while (true) {
//        if(kbhit())
        if (_player->getCoins() <= 0) {
            return false; // TODO: GAME OVER, LET THE ATTACKERS FINISH/DIE
        }
        drawCurrentMoney();
        wtimeout(_game_window, 1000);
        input = wgetch(_game_window);
        if (input != ERR)
            mvprintw(0, 0, "%c", input);
        if (input == 'q') {
            if (pauseMenu()) {
                touchwin(_game_window);
                continue;
            } else
                return false;
        }

        if (input >= '0' && input <= '9')// CHOICES OF INPUT LANES, can be 0-9 (currently using 3)
            _player->setLane(input - 49);
        if (input == 'a') // choices of attacker to spawn
        {
            highlightAttacker(0);
            _player->setAttackerType(0);
        }
        if (input == 's') {
            highlightAttacker(1);
            _player->setAttackerType(1);
        }
        if (input == 'd') {
            highlightAttacker(2);
            _player->setAttackerType(2);
        }
        if (input == ' ') {
            _player->addAttackerToQueue(); // TODO: "CALLBACK" not enough money
        }

        // perform attacks from attackers (we will make it easier for the player)
        // perform attacks from towers
        performAttacks();

        if (!_player->emptyAttackerQueue())
            _player->spawnAttacker();
        // first figure out the route for attackers
        // then add them to ncurses window buffer
        if (!_player->emptyAttackers())
            _player->moveAttackers();
//        wtimeout(_game_window,50);
//        wgetch(_game_window);



        wrefresh(_game_window);
        if (_tower_manager->getTowerCount() == 0)
            break;
    }
    return true; // GAME WON
}


bool CGame::load(int level) {
    if (_gameMap.readMap(level)) {
        return true;
    }
    return false;
}

void CGame::drawTowers() {
    if (_tower_manager->getTowerCount() != 0) {
        _tower_manager->clearTowers();
    }
    _tower_manager->findEmptySpaces();
    _tower_manager->createTowers();
    _tower_manager->printTowers();
    wrefresh(_game_window);
}

void CGame::redrawTowers() {
    if (_tower_manager->getTowerCount() != 0) {
        _tower_manager->clearTowers();
    }
    _tower_manager->printTowers();
    wrefresh(_game_window);
}

bool CGame::save() {
    std::string saveName = CMenu::saveMenu();
    if(saveName.empty())
        return false;
    std::stringstream ss;
    ss << "assets/saves/"<< saveName<< ".txt";
    std::ofstream saveFile;
    saveFile.open(ss.str(), std::ofstream::out| std::ofstream::trunc);
    if(!saveFile.is_open())
        return false;
    if(saveFile.good() == 0)
        return false;
    saveFile << "map " << 1 << std::endl;
    saveFile << "difficulty "<< _difficulty << std::endl;
    saveFile << "tsdestroyed " << _towers_destroyed << std::endl;
    saveFile << "asfinished " << _player->getFinished() << std::endl;
    saveFile << "money " << _player->getCoins() << std::endl;
    saveFile << std::endl;
    saveFile << "# attackers" << std::endl;
    for(const auto & attacker: _player->getAttackers()) {
        auto point = attacker.second->getPosition();
        saveFile << "type " << attacker.second->getTypeName() << std::endl
        << "x " << point.first << std::endl
        << "y " << point.second << std::endl
        << "hp "<< attacker.second->getHP() << std::endl
        << "slw " << attacker.second->getEffects().m_slowEffect << std::endl;
    }
    saveFile << "# towers" << std::endl;
    for(const auto & attacker: _tower_manager->getTowers()) {
        auto point = attacker.second->getPosition();
        saveFile << "type " << attacker.second->getTypeName() << std::endl
                 << "x " << point.first << std::endl
                 << "y " << point.second << std::endl
                 << "hp "<< attacker.second->getHP() << std::endl;
    }
    saveFile << "# queue" << std::endl;
    for(const auto & attacker: _player->getAttackersQueue()) {
        auto point = attacker.second;
        saveFile << "name " << attacker.first << std::endl
                 << "x " << point._x << std::endl
                 << "y " << point._y << std::endl;
    }
    saveFile << "# end" << std::endl;
    saveFile<< std::flush;
    saveFile.close();
    return true;
}
// loads game from file defined in the parameter and starts the game
bool CGame::loadFromSave(const std::string &nameOfSave) {
    // delete all towers and players
    if(_player != nullptr)
        _player->clearAttackers();
    if(_tower_manager!= nullptr)
        _tower_manager->clearTowers();
    // delete the old map and load the map definition provided in the save file
    _gameMap.clearMap();
    std::stringstream savepath;
    savepath <<"assets/saves/"<<nameOfSave;
    std::ifstream savefile(savepath.str(), std::ios::in);
    std::string fileLine;
    std::map<std::string,std::vector<std::map<std::string, std::string>>> entities;
    std::map<std::string,std::string> givenEntity;
    std::vector<std::map<std::string, std::string>> vectorOfEntities;
    std::map<std::string, std::string> mapOfAttackersInQueue;
    std::deque<std::pair<std::string, Point>> attackersInQueue;
    std::map<std::string, int> gameStats;
    std::string name;
    std::string value;
    bool gameReading = false;
    bool queueReading = false;
    int lineindex = 0;
    std::string entType;
    int indexOfEnt;
    std::string test;
    while (getline(savefile, fileLine))
    {
        lineindex++;
        if(fileLine.empty())
            continue;
        std::stringstream ss(fileLine);
        if(!gameReading){
            int numvalue=-10;
            name.clear();
            ss >> name;
            ss >> numvalue;
            if(numvalue == -10) {
                ss.str(std::string());
                ss << "attribute on line " << lineindex << " in save file " << nameOfSave  <<" is incorrect. " << "name= " << name << " value= " << value;
                throw syntaxErr(ss.str());
            }
            gameStats.emplace(name,numvalue);
            if(lineindex==5)
                gameReading = true;
            continue;
        }
        if(!queueReading){
            if( fileLine[0] == '#'){
                if(!givenEntity.empty()) {
                    vectorOfEntities.emplace_back(givenEntity);
                    givenEntity.clear();
                }
                if(!vectorOfEntities.empty()) {
                    entities.emplace(entType, vectorOfEntities);
                    vectorOfEntities.clear();
                }
                test.clear();
                ss >> entType; // getting rid of #
                entType.clear();
                ss >> entType; // value of type
                if(entType=="queue") {
                    queueReading = true;
                    continue;
                }
                ss >> test;
                if(!test.empty() || entType.empty())
                {
                    ss.str(std::string());
                    ss << "attribute on line " << lineindex << " in save file " << nameOfSave  <<" is incorrect.";
                    throw syntaxErr(ss.str());
                }
                continue;
            }
            name.clear();
            value.clear();
            test.clear();
            ss >> name;
            ss >> value;
            ss >> test;
            if(!test.empty() || value.empty() || name.empty())
            {
                ss.str(std::string());
                ss << "attribute on line " << lineindex << " in save file " << nameOfSave  <<" is incorrect. "<< "name= " << name << " value= " << value;
                throw syntaxErr(ss.str());
            }
            if(name == "type"){
                if(!givenEntity.empty()) {
                    vectorOfEntities.emplace_back(givenEntity);
                    givenEntity.clear();
                }
            }
            givenEntity.emplace(name,value);
        }else{
            int number;
            name.clear();
            value.clear();
            test.clear();
            ss >> name;
            ss >> value;
            ss >> test;
            if(!test.empty() || name.empty() || value.empty())
            {
                ss.str(std::string());
                ss << "attribute of queue on line" << lineindex << " in save file " << nameOfSave  <<" is incorrect. "<< "name= " << name << " value= " << value;
                throw syntaxErr(ss.str());
            }
            if(name == "name" || (name == "#" && value == "end"))
            {
                if(!mapOfAttackersInQueue.empty()){
                    std::string attrName;
                    int attrX;
                    int attrY;
                    if(mapOfAttackersInQueue.count("name")==0 ||mapOfAttackersInQueue.count("x")==0 || mapOfAttackersInQueue.count("y")==0)
                    {
                        ss.str(std::string());
                        ss << "one or more attributes in queue in save file " << nameOfSave  <<" is incorrect.";
                        throw syntaxErr(ss.str());
                    }
                    try{
                        attrName = mapOfAttackersInQueue.at("name");
                        attrX = std::stoi(mapOfAttackersInQueue.at("x"));
                        attrY = std::stoi(mapOfAttackersInQueue.at("y"));
                    }catch(std::exception &e){
                        ss.str(std::string());
                        ss << "one or more attributes in queue in save file " << nameOfSave  <<" is incorrect.";
                        throw syntaxErr(ss.str());
                    }
                    attackersInQueue.emplace_back(std::make_pair(attrName, Point(attrX,attrY)));
                    mapOfAttackersInQueue.clear();
                }
                if(name == "#" && value == "end")
                    break;
            }
            mapOfAttackersInQueue.emplace(name, value);
        }
    }

    int i{};
    for(const auto & type: entities){
        sendToLogFile(0,"loading entities typed "+type.first,"loadFromSave");
        i = 0;
        for (const auto & entity: type.second){
            i++;
            sendToLogFile(0,"   index "+std::to_string(i), "loadFromSave");
            for (const auto & attr: entity){
                sendToLogFile(0,"      attibute "+attr.first+ " valued " + attr.second, "loadFromFile");
            }
        }
    }


    // load the safe file into a respective vectors of of attackers and towers
    if(entities.count("attackers") == 1 ) // entities.count("towers") == 1
    {
        for(const auto& utocnici: entities["attackers"]){
            int x;
            int y;
            int hp;
            CEffects eff;
            std::stringstream ss;
            try{
                x = std::stoi(utocnici.at("x"));
                y = std::stoi(utocnici.at("y"));
                hp = std::stoi(utocnici.at("hp"));
                eff.m_slowEffect = std::stoi(utocnici.at("slw"));
            }catch(std::exception& e)
            {
                ss.str(std::string());
                ss << "one or more attributes is incorrect, saveFile might be corrupt";
                throw syntaxErr(ss.str());
            }
            if(utocnici.at("type") == BASICA)
                _player->createNewAttacker(0,x,y,hp,eff);
            else if(utocnici.at("type") == FASTA)
                _player->createNewAttacker(1,x,y,hp,eff);
            else if(utocnici.at("type") == CHARGERA)
                _player->createNewAttacker(2,x,y,hp,eff);
            else{
                ss.str(std::string());
                ss << "one or more attributes is incorrect, saveFile might be corrupt";
                throw syntaxErr(ss.str());
            }
        }
    }
    if(entities.count("towers") == 1 ){
        for(const auto& veze: entities["towers"]){
            int x;
            int y;
            int hp;
            std::stringstream ss;
            try{
                x = std::stoi(veze.at("x"));
                y = std::stoi(veze.at("y"));
                hp = std::stoi(veze.at("hp"));
            }catch(std::invalid_argument& e)
            {
                ss.str(std::string());
                ss << "one or more attributes is incorrect, saveFile might be corrupt";
                throw syntaxErr(ss.str());
            }
            catch(std::out_of_range& e)
            {
                ss.str(std::string());
                ss << "one or more attributes is missing, saveFile might be corrupt";
                throw syntaxErr(ss.str());
            }
            if(veze.at("type") == BASICT)
                _tower_manager->createNewTower(0,x,y,hp);
            else if(veze.at("type") == FASTT)
                _tower_manager->createNewTower(1,x,y,hp);
            else if(veze.at("type") == STRONGT)
                _tower_manager->createNewTower(2,x,y,hp);
            else if(veze.at("type") == SLOWET)
                _tower_manager->createNewTower(3,x,y,hp);
            else{
                ss.str(std::string());
                ss << "one or more attributes is incorrect, saveFile might be corrupt";
                throw syntaxErr(ss.str());
            }
    }

    }


    // read the map
    _gameMap.readMap(gameStats["map"]);
    // print towers and attackers on screen and start the game
    _tower_manager->printTowers();
    _player->printAttackers();
    _player->setAttackersQueue(attackersInQueue);
    // set the game money and difficulty to the one specified in the save file
    if(gameStats.count("money")==0 || gameStats.count("money") == 0 || gameStats.count("asfinished") == 0 || gameStats.count("tsdestroyed") == 0)
        throw syntaxErr("one or more gamestats is invalid, saveFile might be corrupt");
    _player->setCoins(gameStats["money"]);
    setDifficulty(gameStats["difficulty"]);
    _player->setFinished(gameStats["asfinished"]);
    setTowersDestroyed(gameStats.at("tsdestroyed"));
    return true;
}

bool CGame::exit() {
    throw (notImplementedException("exit"));
}


void hell() {
    attron(A_STANDOUT);
    printw("welcome to hell");
    attroff(A_STANDOUT);
}


void CGame::highlightAttacker(int type) {
    int top = (int) _gameMap.m_map.size() + 2;
    int defaultTop = top;
    int left = 2;
    int i = 0;
    for (const auto &ent: _definitions.getAttacker()) {
        if (i == type)
            wattron(_game_window, A_STANDOUT);
        mvwprintw(_game_window, top, left, "%s", ent.first.c_str());
        top++;
        for (auto &hodnoty: ent.second) {
            top++;
            if (hodnoty.first == "symbol") {
                mvwprintw(_game_window, top, left, "%s : %c", hodnoty.first.c_str(), hodnoty.second);
                continue;
            }
            mvwprintw(_game_window, top, left, "%s : %d", hodnoty.first.c_str(), hodnoty.second);
        }
        top = defaultTop;
        left += (int) ent.first.length() + 4;
        if (i == type)
            wattroff(_game_window, A_STANDOUT);
        i++;
    }
    wrefresh(_game_window);
}

void CGame::drawAttackerDefs() {
    int top = (int) _gameMap.m_map.size() + 2;
    int defaultTop = top;
    int left = 2;

    for (const auto &ent: _definitions.getAttacker()) {
        mvwprintw(_game_window, top, left, "%s", ent.first.c_str());
        top++;
        for (auto &hodnoty: ent.second) {
            top++;
            if (hodnoty.first == "symbol") {
                mvwprintw(_game_window, top, left, "%s : %c", hodnoty.first.c_str(), hodnoty.second);
                continue;
            }
            mvwprintw(_game_window, top, left, "%s : %d", hodnoty.first.c_str(), hodnoty.second);
        }
        top = defaultTop;
        left += (int) ent.first.length() + 4;
    }
    wrefresh(_game_window);
}

// display the stats of the game and return to main menu
bool CGame::gameEnd(const char *msg) {
    CMenu endMenu;
    std::stringstream scorestring;
    std::stringstream sstowersDestroyed;

    _score = (_towers_destroyed * 200
              + _player->getCoins() * 100 + _player->getFinished()*25) * _difficulty;

    scorestring << "Your score is: " << _score;
    sstowersDestroyed << "You have destroyed: " << _towers_destroyed << " towers";
    endMenu.setMenu({"Game Over!"," ", scorestring.str(),sstowersDestroyed.str()}, {"QUIT", "RETURN TO MENU"});

    int retval = endMenu.show();
    if(retval == 0)
        return false;
    return true;
}

bool CGame::init(int level, int difficulty, const char *nameOfSave) {
    _difficulty = difficulty;
    if (_definitions.getTower().empty())
        _definitions.loadDefinitions();
    terminal term;
    int winheight = 25;
    int winwidth = 75;
    getmaxyx(stdscr, term.height, term.width); // zjisteni velikosti obrazovky
    int starty = (term.height - winheight) / 2; // vypocet pozice mapy
    int startx = (term.width - winwidth) / 2; // vypocet pozice mapy
    if(_game_window == nullptr) {
        _game_window = newwin(winheight, winwidth, starty, startx);
        _gameMap.setWindow(_game_window);
        initializeWindow();
    }else{
        delwin(_game_window);
        _game_window = newwin(winheight, winwidth, starty, startx);
        _gameMap.setWindow(_game_window);
        initializeWindow();
    }

    if (_player == nullptr)
        _player = new Player(&_gameMap, _definitions.getAttacker());
    else
        _player->clearAttackers();

    if (_tower_manager == nullptr)
        _tower_manager = new Enemy(&_gameMap, _definitions.getTower(), _difficulty);
    else
        _tower_manager->clearTowers();

    if (std::strlen(nameOfSave)!=0) {
        loadFromSave(nameOfSave);
    } else {
        if (_gameMap.m_map.empty())
            load(level);
        drawTowers();
        _player->setCoins(2500);
    }
    _gameMap.setWindow(_game_window);
    return true;
}

bool CGame::play() {
    initscr(); // inicializace ncurses a pameti
    noecho(); // zakaz vypisu na obrazovku
    cbreak(); // ctrlc se neukonci program


    while (true) {
        int choice = CMenu::mainMenu();

        if (choice == 0) { // choice is new game
            //new game
            if (!this->init(1, _difficulty))
                throw logException("Game init failed");
            if (!this->start()) {
                if (this->gameEnd("You lost!"))
                    continue;
                else
                    return EXIT_FAILURE;
            } else {
                if (this->gameEnd("You won!"))
                    continue;
                else
                    return EXIT_FAILURE;
            }
        } else if (choice == 1) { // load game
            std::vector<std::string> saves;
            choice = CMenu::loadMenu(saves);
            std::string save;
            if (choice >= 0) {
                save = saves[choice];
            }else if (choice == -20)
                continue;

            this->init(1, _difficulty, save.c_str());
            if (!this->start()) {
                if (this->gameEnd("You lost!"))
                    continue;
                else
                    return EXIT_FAILURE;
            } else {
                if (this->gameEnd("You won!"))
                    continue;
                else
                    return EXIT_FAILURE;
            }
        }else if(choice == 2)
        {
            _difficulty = CMenu::optionsMenu() + 1;
            continue;
        } else if (choice == 3) {
            //exit
            break;
        }
    }
    return EXIT_SUCCESS;
}

bool CGame::pauseMenu(const char *msg) {
    CMenu pauseMenu({"GAME PAUSED", msg}, {"Resume", "Save and resume", "End and don't save"});

    int choice = pauseMenu.show();
    if (choice == 0) { // resume game
        return true;
    } else if (choice == 1) {
        if (!save())
            return this->pauseMenu("Save failed, try again.");
        return true; // game saved and resumed
    } else if (choice == 2) {
        return false; // quit game
    }
    return false; // TODO: HANDLE RETURN VALUES
}

void CGame::drawCurrentMoney() {
    std::stringstream ss;
    ss << "Money: " << _player->getCoins() << " ";
    int width = _gameMap.getMapWidth();
    mvwprintw(_game_window, 2, width + 2, ss.str().c_str());
}

bool CGame::performAttacks() {
    auto towersToAttack = _player->getTowersToAttack();

    if(!towersToAttack.empty())
        _towers_destroyed+= _tower_manager->damageTowers(towersToAttack);

    auto attackersToAttack = _tower_manager->getAttackersToAttack();
    if(!attackersToAttack.empty())
        _player->damageAttackers(attackersToAttack);

    for(const auto & attacker: attackersToAttack){
        sendToLogFile(0, "performing attack on Attacker ID:"+std::to_string(attacker.first), "CGame::performAttacks");
    }
    for(const auto & tower: towersToAttack){
        sendToLogFile(0, "performing attack on Tower ID:"+std::to_string(tower.first), "CGame::performAttacks");
    }

    return true;
}

void CGame::setDifficulty(int def) {
    _difficulty = def;
}

void CGame::setTowersDestroyed(int nr) {
    _towers_destroyed=nr;
}
