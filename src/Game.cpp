#include "Game.h"


CGame::~CGame() {
    delete _player;
    delete _tower_manager;
    delwin(_game_window); // zavreni okna s hrou
    endwin(); // dealoc pameti a zavreni ncurses
}

bool CGame::start() {
    _gameMap.redrawMap();
    drawTowers();
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
    _player->setCoins(2500);
    while (true) {
//        if(kbhit())
        if (_player->getCoins() <= 0) {
            return false; // GAME OVER, LET THE ATTACKERS FINISH/DIE
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
            _player->addAttackerToQueue();
        }
        if (!_player->emptyAttackerQueue())
            _player->spawnAttacker();
        // first figure out the route for attackers
        // then add them to ncurses window buffer
        if (!_player->emptyAttackers())
            _player->moveAttackers();
//        wtimeout(_game_window,50);
//        wgetch(_game_window);

        performAttacks();
        // perform attacks from attackers (we will make it easier for the player)
        // perform attacks from towers
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
    //
    throw (notImplementedException("save"));
}

bool CGame::loadFromSave(const std::string &path) {
    // delete all towers and players
    _player->clearAttackers();
    _tower_manager->clearTowers();
    // delete the old map and load the map definition provided in the save file
    _gameMap.clearMap();
    // load the safe file into a respective vectors of of attackers and towers

    // print the map, towers and attackers on screen and start the game
    // set the game money to the one specified in the save file
    // set the difficulty to the one specified in the save file
    // delete the definitions and load them again
    // set the game score to the one specified in the save file

    throw (notImplementedException("loadFromSave"));
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

    _score = (towers_destroyed * 200
              + _player->getCoins() * 100 + _player->getFinished()*25) * _difficulty;

    scorestring << "Your score is: " << _score;
    endMenu.setMenu({"Game Over!"," ", scorestring.str()}, {"QUIT", "RETURN TO MENU"});

    int retval = endMenu.show();
    if(retval == 0)
        return false;
    return true;
}

bool CGame::init(int level, int difficulty, const char *pathToSave) {
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
    }
    if (level == 0) {
        loadFromSave(pathToSave);
    } else {
        if (_gameMap.m_map.empty())
            load(level);
    }

    if (_player == nullptr)
        _player = new Player(&_gameMap, _definitions.getAttacker());
    else
        _player->clearAttackers();
    if (_tower_manager == nullptr)
        _tower_manager = new Enemy(&_gameMap, _definitions.getTower(), _difficulty);
    else
        _tower_manager->clearTowers();
    return true;
}

bool CGame::play() {
    initscr(); // inicializace ncurses a pameti
    noecho(); // zakaz vypisu na obrazovku
    cbreak(); // ctrlc se neukonci program


    while (true) {
        int choice = mainMenu();

        if (choice == 0) { // choice is new game
            //new game
            if (!this->init(1, 1))
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
            choice = loadMenu(saves);
            std::string save;
            if (choice > 0) {
                save = saves[choice];
            } else save = "";

            this->init(1, 1, save.c_str());
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

        } else if (choice == 2) {
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
    size_t sizeBefore = _tower_manager->getTowerCount();
    if(!towersToAttack.empty())
        _tower_manager->damageTowers(towersToAttack);

    size_t sizeAfter = _tower_manager->getTowerCount();
    towers_destroyed+= sizeBefore - sizeAfter;
    return true;
}
