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
    shared_ptr<Map> mapa;
    auto test = new basicAttacker(0,0,100,mapa);

    terminal term;

    initscr(); // inicializace ncurses a pameti
    noecho(); // zakaz vypisu na obrazovku
    cbreak(); // ctrlc se neukonci program

    int choice = newMenu(CMenu({"Welcome to", "The Tower Attack 2 ", "Electric Boogaloo"},{"New Game", "Load Game", "Exit"}));
//    int choice = mainMenu(term);

    if(choice == 0){
        //new game
        CGame game(1,1);
        game.start();
    }
    else if(choice == 1){
        // load game
        std::vector<string> saves;
        choice = loadMenu(saves);
    }
    else if(choice == 2){
        //exit
        endwin();
        return EXIT_SUCCESS;
    }


    string map;
    mapa->printMap(map);
    printw(map.c_str());
    getch(); // cekani na stisk klavesy

    mapa->updateMap(0,0,5,5,test);
    delete test;
    getch();
    endwin(); // dealoc pameti a zavreni ncurses

    return 0;
}

