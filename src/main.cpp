//
// Created by Štěpán on 27.04.2022.
//
#include <ncurses.h>
#include "Entity.h"
#include "Attacker.h"
#include "Map.h"

using namespace std;

int main (int argc, char **argv){

    auto test = new basicAttacker(0,0,100);
    Map mapa;
    initscr(); // inicializace ncurses a pameti
    refresh(); // zobrazeni okna podle toho co je v pameti
//    printw("Hello world!"); // vypis textu
    string map;
    map += test->getSymbol();
    mapa.printMap(map);
    printw(map.c_str());
    getch(); // cekani na stisk klavesy
    endwin(); // dealoc pameti a zavreni ncurses

    return 0;
}
