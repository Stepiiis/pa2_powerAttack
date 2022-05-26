#pragma once
//
// Created by Štěpán on 27.04.2022.
//
#include <ncurses.h>
#include "Entity.h"
#include "Attacker.h"
#include "Definitions.h"
#include "Map.h"
#include "Game.h"

using namespace std;


int main (int argc, char **argv){

    int money = 100;
    terminal term;


    initscr(); // inicializace ncurses a pameti
    noecho(); // zakaz vypisu na obrazovku
    cbreak(); // ctrlc se neukonci program


//    int choice = newMenu(CMenu({"Welcome to", "The Tower Attack 2 ", "Electric Boogaloo"},{"New Game", "Load Game", "Exit"}));
    int choice = mainMenu();

    if(choice == 0){
        //new game
        CGame game(1,1);
        game.start();
    }
    else if(choice == 1){
        // load game
        std::vector<string> saves;
        choice = loadMenu(saves);
        std::string save;
        if(choice >0) {
            save = saves[choice];
        }else
        {
            save = "";
        }
        CGame game(1,1,save);
        game.start();


    }
    else if(choice == 2){
        //exit
        endwin();
        return EXIT_SUCCESS;
    }


    endwin(); // dealoc pameti a zavreni ncurses

    return 0;
}

