#include "Game.h"
#include <ncurses.h>
#include <iostream>
void hell(){
    attron(A_STANDOUT);
    printw("welcome to hell");
    attroff(A_STANDOUT);
}
CMenu::CMenu(std::initializer_list<std::string> text, std::initializer_list<std::string> options)
{
    _text = std::move(text);
    _options = std::move(options);
}


int newMenu(CMenu && menu){
    terminal term;
    int height = 25;
    int width = 75;
    int posNew = 5;
    int posLoad = 6;
    int posExit = 7;
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
    posY++;
    mvwprintw(menu_win, posY, posX, "Press any key to continue");
    printShrek(menu_win, posY+1,posX);
    refresh();
    wgetch(menu_win);
    mvwprintw(menu_win, posY, posX, "                         ");
    clearShrek(menu_win, posY+1,posX);
    posY++;
    int temp = posY;
    while(true){
        posY = temp;
        keypress = wgetch(menu_win);
        if (keypress == KEY_UP) {
            choice = (choice - 1) ;
            if(choice == -1)
                choice =  menu._options.size() - 1;
            else
                choice %= menu._options.size();
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

    touchwin(stdscr);
    refresh();
    move(0,0);
    return choice;
}

int mainMenu(terminal term){
    int height = 50;
    int width = 100;
    int posNew = 5;
    int posLoad = 6;
    int posExit = 7;
    int posX = 16;

    auto style = A_STANDOUT;
    attron(style);
    printw("welcome to hell");
    attroff(style);
    refresh();
    getmaxyx(stdscr, term.height, term.width); // zjisteni velikosti obrazovky
    int starty = (term.height - height) / 2; // vypocet pozice mapy
    int startx = (term.width - width )/ 2; // vypocet pozice mapy
    auto menu_win = newwin(height, width, starty, startx);
    refresh();
    keypad(menu_win, TRUE);
    box(menu_win, 0, 0);
    wrefresh(menu_win);
    mvwprintw(menu_win, 1, 1, "\t\tWelcome to");
    mvwprintw(menu_win,2,1,"\t\tTowerAttack 2");
    mvwprintw(menu_win,3,1,"\t\tElectric Boogaloo!");
    wattron(menu_win,style);
    mvwprintw(menu_win, posNew, posX, "NEW GAME");
    wattroff(menu_win,style);
    wgetch(menu_win);
    mvwprintw(menu_win, posLoad, posX, "LOAD GAME");
    mvwprintw(menu_win, posExit, posX, "EXIT");
    wrefresh(menu_win);
    int choice = 0;
    int keypress;
    while(true){
        keypress = wgetch(menu_win);
        if (keypress == KEY_UP) {
            choice = (choice - 1) % 3;
            if(choice == -1) choice = 2;
        } else if (keypress == KEY_DOWN) {
            choice = (choice + 1) % 3;
        } else if (keypress == KEY_DC) {
            break;
        }
        if(choice == 0) {
//            cout << "new game" << endl;
            move(starty + posNew, startx + posX);
            wattron(menu_win,style);
            mvwprintw(menu_win, posNew, posX, "NEW GAME");
            wattroff(menu_win,style);
            mvwprintw(menu_win, posLoad, posX, "LOAD GAME");
            mvwprintw(menu_win, posExit, posX, "EXIT");
            wrefresh(menu_win);
            continue;
        }else if(choice == 1) {
//            cout<< "load game" << endl;
            move(starty + posLoad, startx + posX);
            mvwprintw(menu_win, posNew, posX, "NEW GAME");
            wattron(menu_win,style);
            mvwprintw(menu_win, posLoad, posX, "LOAD GAME");
            wattroff(menu_win,style);
            mvwprintw(menu_win, posExit, posX, "EXIT");
            wrefresh(menu_win);
            continue;
        }else if(choice == 2) {
//            cout << "exit" << endl;
            move(starty + posExit, startx + posX);
            mvwprintw(menu_win, posNew, posX, "NEW GAME");
            mvwprintw(menu_win, posLoad, posX, "LOAD GAME");
            wattron(menu_win,style);
            mvwprintw(menu_win, posExit, posX, "EXIT");
            wattroff(menu_win,style);
            wrefresh(menu_win);
            continue;
        }
    }

    touchwin(stdscr);
    refresh();
    move(0,0);
    return choice;
}

CGame::CGame(int){}
CGame::~CGame(){}
bool CGame::start(){}
bool CGame::save(){}
bool CGame::load(){}
bool CGame::exit(){}

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
