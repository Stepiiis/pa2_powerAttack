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

    initscr(); // inicializace ncurses a pameti
    noecho(); // zakaz vypisu na obrazovku
    cbreak(); // ctrlc se neukonci program
    std::vector<string> text();
    std::vector<string> options ();

    int choice = newMenu(CMenu({"Welcome to", "The Tower Attack 2 ", "Electric Boogaloo"},{"New Game", "Load Game", "Exit"}));
//    int choice = mainMenu(term);

    if(choice == 0){
        //new game
        CGame game(0);
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

