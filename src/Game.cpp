#include "Game.h"
#include <ncurses.h>
int mainMenu(terminal term){
    int height = 25;
    int width = 50;
    int posNew = 5;
    int posLoad = 6;
    int posExit = 7;
    int posX = 16;
    initscr(); // inicializace ncurses a pameti
    noecho(); // zakaz vypisu na obrazovku
    cbreak(); // ctrlc se neukonci program
    auto style = A_STANDOUT;
    attron(style);
    printw("welcome to hell");
    attroff(style);
    refresh();
    getch();
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