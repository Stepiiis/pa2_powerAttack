#pragma once
//
// Created by Štěpán on 27.04.2022.
//
#include <ncurses.h>
#include "Entity.h"
#include "Attacker.h"
#include "Map.h"
#include "Game.h"

using namespace std;


int main (int argc, char **argv){

    int money = 100;
    auto test = new basicAttacker(0,0,100);

    terminal term;

    int choice = mainMenu(term);



    Map mapa;
    string map;
    mapa.printMap(map);
    printw(map.c_str());
    getch(); // cekani na stisk klavesy

    mapa.updateMap(5,5,test);
    getch();
    endwin(); // dealoc pameti a zavreni ncurses

    return 0;
}
