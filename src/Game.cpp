#include "Game.h"
#include <ncurses.h>
#include <iostream>
#include <memory>
#include <sys/ioctl.h>
#include <experimental/filesystem>
#include "Definitions.h"


CGame::CGame(int level,  int difficulty, const std::string & pathToSave )
        : _difficulty(difficulty), _save_name(pathToSave)
{
    _definitions.loadDefinitions();
    terminal term;
    int winheight = 25;
    int winwidth = 75;
    getmaxyx(stdscr, term.height, term.width); // zjisteni velikosti obrazovky
    int starty = (term.height - winheight) / 2; // vypocet pozice mapy
    int startx = (term.width - winwidth )/ 2; // vypocet pozice mapy
    _game_window = newwin(winheight, winwidth, starty, startx);
    _gameMap.setWindow(_game_window);
    initializeWindow();
    if(level == 0){
        loadFromSave(pathToSave);
    } else {
        load(level);
    }
    _player = new Player(&_gameMap, _definitions.getAttacker());
    _tower_manager = new Enemy(&_gameMap,_definitions.getTower(), _difficulty);
}

CGame::~CGame(){
    delete _player;
    delete _tower_manager;
    delwin(_game_window); // zavreni okna s hrou
    endwin(); // dealoc pameti a zavreni ncurses
}

bool CGame::start(){
    _gameMap.redrawMap();   
    drawTowers();
    drawAttackerDefs();
    if(resume())
        return true;
    // TODO: HANDLE GAME LOSS
    return false;
}

bool CGame::initializeWindow(){
    auto style = A_STANDOUT;
    refresh();
    keypad(_game_window, TRUE);
    box(_game_window, 0, 0);
    wrefresh(_game_window);
    return true;
}

// prevzato ze StackOverflow
// https://stackoverflow.com/questions/421860/capture-characters-from-standard-input-without-waiting-for-enter-to-be-pressed/67363091#67363091
int kbhit(){
    static int nbbytes;
    ioctl(0, FIONREAD, &nbbytes);
    return nbbytes;
}
// konec prevzateho

bool CGame::resume()
{
    int input;
    _player->setCoins(2500);
    while(true){
//        if(kbhit())
        if(_player->getCoins() <= 0){
            return false; // GAME OVER, LET THE ATTACKERS FINISH/DIE
        }
        wtimeout(_game_window,1000);
        input = wgetch(_game_window);
        if(input != ERR)
            mvprintw(0,0,"%c", input);
        if(input == 'q'){
            break;
        }

        if(input >= '0' && input <= '9')// CHOICES OF INPUT LANES, can be 0-9 (currently using 3)
            _player->setLane(input-49);
        if(input == 'a') // choices of attacker to spawn
        {
            highlightAttacker(0);
            _player->setAttackerType(0);
        }
        if(input == 's')
        {
            highlightAttacker(1);
            _player->setAttackerType(1);
        }
        if(input == 'd')
        {
            highlightAttacker(2);
            _player->setAttackerType(2);
        }
        if(input == ' ')
        {
            _player->addAttackerToQueue();
        }
        if(!_player->emptyAttackerQueue())
            _player->spawnAttacker();
        if(!_player->emptyAttackers())
            _player->moveAttackers();
//        wtimeout(_game_window,50);
//        wgetch(_game_window);
        wrefresh(_game_window);
        // first figure out the route for attackers
        // then add them to ncurses window buffer
        // refresh screen at the end !!!
        // perform attacks from attackers (we will make it easier for the player)
        // perform attacks from towers
    }
    return true; // GAME WON
}



bool CGame::load(int level){
    if(_gameMap.readMap(level)){
        return true;
    }
    return false;
}

void CGame::drawTowers(){
    if(_tower_manager->getTowerCount() != 0) {
        _tower_manager->clearTowers();
    }
    _tower_manager->findEmptySpaces();
    _tower_manager->createTowers();
    _tower_manager->printTowers();
    wrefresh(_game_window);
}
bool CGame::save(){
    throw(notImplementedException("save"));
}
bool CGame::loadFromSave(std::string path){
    throw(notImplementedException("loadFromSave"));
}
bool CGame::exit(){
    throw(notImplementedException("exit"));
}



void hell(){
    attron(A_STANDOUT);
    printw("welcome to hell");
    attroff(A_STANDOUT);
}

CMenu::CMenu(std::vector<std::string> text, std::vector<std::string> options)
{
    _text = std::move(text);
    _options = std::move(options);
}

int newMenu(const CMenu & menu){
    terminal term;
    int height = 25;
    int width = 75;
    int posX = (width/2) - (menu._text.size()/2) - 10;

    auto style = A_STANDOUT;
    getmaxyx(stdscr, term.height, term.width); // zjisteni velikosti obrazovky
    int starty = (term.height - height) / 2; // vypocet pozice mapy
    int startx = (term.width - width )/ 2; // vypocet pozice mapy
    auto menu_win = newwin(height, width, starty, startx);
    refresh();
    keypad(menu_win, TRUE);
    box(menu_win, 0, 0);
    wrefresh(menu_win);
    int posY = 5;
    auto text = menu._text.begin();
    while(text != menu._text.end()){
        mvwprintw(menu_win, posY, posX, "%s", (*text).c_str());
        wrefresh(menu_win);
        posY++;
        text++;
    }
    wrefresh(menu_win);
    int keypress;
    int choice = 0;
//    posY++;
//    mvwprintw(menu_win, posY, posX, "Press any key to continue");
//    printShrek(menu_win, posY+1,posX);
//    refresh();
//    wgetch(menu_win);
//    mvwprintw(menu_win, posY, posX, "                         ");
//    clearShrek(menu_win, posY+1,posX);
    posY++;
    int temp = posY;
    while(true){
        posY = temp;
        keypress = wgetch(menu_win);
        if (keypress == KEY_UP) {
            choice = (choice - 1) ;
            if(choice == -1)
                choice =  (int)menu._options.size() - 1;
            else
                choice %= (int)menu._options.size();
        } else if (keypress == KEY_DOWN) {
            choice = (choice + 1) % menu._options.size();
        } else if (keypress == KEY_DC) {
            break;
        }
//        wmove(menu_win,0,0);
//        wprintw(menu_win, "%d %d %d", choice, menu._options.size()), (-1%3);
        auto option = menu._options.begin();
        while(option != menu._options.end()){
            if(choice == std::distance(menu._options.begin(), option)){
                wattron(menu_win, style);
                mvwprintw(menu_win, posY, posX, "%s", (*option).c_str());
                wattroff(menu_win, style);
            } else {
                mvwprintw(menu_win, posY, posX, "%s", (*option).c_str());
            }
            posY++;
            option++;
        }
        wrefresh(menu_win);

    }
    delwin(menu_win);
    touchwin(stdscr);
    refresh();
    move(0,0);
    return choice;
}

int mainMenu(){

    return newMenu(CMenu({
                                      "Welcome to",
                                      "Tower Attack 2",
                                      "Electric Boogaloo"
                              },{
                                      "New game",
                                      "Load game",
                                      "Exit"
                              }));
}
int loadMenu(std::vector<std::string>& save_names){
    const std::experimental::filesystem::path path {"data/saves/"};
    for(const auto & saves: std::experimental::filesystem::directory_iterator(path)){
        save_names.emplace_back(saves.path().filename().string());
//        std::cout << saves.path().filename().string() << std::endl;
    }
    if(!save_names.empty())
        return newMenu(CMenu({
                                          "Please select a save file",
                                          "and confirm with delete"
                                  },save_names));

    newMenu(CMenu({
                                      "No save files found",
                                      "Please create a new game"
                              },{
                                      "New game"
                              }));
    return -10;
}


void printShrek(WINDOW* menu_win, int posY, int posX){
    mvwprintw(menu_win, posY++, posX,  "                         ");
    mvwprintw(menu_win, posY++, posX,  "      @     |    @       ");
    mvwprintw(menu_win, posY++, posX,  "      @     |    @  @    ");
    mvwprintw(menu_win, posY++, posX,  "      @     |    @  @    ");
    mvwprintw(menu_win, posY++, posX,  "            |            ");
    mvwprintw(menu_win, posY++, posX,  "------------+------------");
    mvwprintw(menu_win, posY++, posX,  "            |            ");
    mvwprintw(menu_win, posY++, posX,  "    @   @   |   @        ");
    mvwprintw(menu_win, posY++, posX,  "    @   @   |   @        ");
    mvwprintw(menu_win, posY, posX,    "    @   @   |   @ @@@@@  ");
}

void clearShrek(WINDOW* menu_win, int posY, int posX){
    mvwprintw(menu_win, posY++, posX,  "                         ");
    mvwprintw(menu_win, posY++, posX,  "                         ");
    mvwprintw(menu_win, posY++, posX,  "                         ");
    mvwprintw(menu_win, posY++, posX,  "                         ");
    mvwprintw(menu_win, posY++, posX,  "                         ");
    mvwprintw(menu_win, posY++, posX,  "                         ");
    mvwprintw(menu_win, posY++, posX,  "                         ");
    mvwprintw(menu_win, posY++, posX,  "                         ");
    mvwprintw(menu_win, posY++, posX,  "                         ");
    mvwprintw(menu_win, posY, posX,    "                         ");
}

void CGame::highlightAttacker(int type) {
    int top = _gameMap.m_map.size() + 2 ;
    int defaultTop = top;
    int left = 2;
    int i = 0;
    for(const auto & ent: _definitions.getAttacker())
    {
        if(i == type)
            wattron(_game_window,A_STANDOUT);
        mvwprintw(_game_window,top,left,"%s",ent.first.c_str());
        top++;
        for(auto & hodnoty: ent.second){
            top++;
            if(hodnoty.first == "symbol") {
                mvwprintw(_game_window,top,left,"%s : %c",hodnoty.first.c_str(), hodnoty.second);
                continue;
            }
            mvwprintw(_game_window,top,left,"%s : %d",hodnoty.first.c_str(), hodnoty.second);
        }
        top=defaultTop;
        left += ent.first.length()+4;
        if(i == type)
                wattroff(_game_window,A_STANDOUT);
        i++;
    }
    wrefresh(_game_window);
}

void CGame::drawAttackerDefs() {
    int top = _gameMap.m_map.size() + 2 ;
    int defaultTop = top;
    int left = 2;

    for(const auto & ent: _definitions.getAttacker())
    {
        mvwprintw(_game_window,top,left,"%s",ent.first.c_str());
        top++;
        for(auto & hodnoty: ent.second){
            top++;
            if(hodnoty.first == "symbol") {
                mvwprintw(_game_window,top,left,"%s : %c",hodnoty.first.c_str(), hodnoty.second);
                continue;
            }
            mvwprintw(_game_window,top,left,"%s : %d",hodnoty.first.c_str(), hodnoty.second);
        }
        top=defaultTop;
        left += ent.first.length()+4;
    }
    wrefresh(_game_window);
}
