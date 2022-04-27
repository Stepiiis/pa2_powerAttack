//
// Created by Štěpán on 27.04.2022.
//
#include <ncurses>
using namespace std;

int main (int argc, char **argv){

    initscr(); // inicializace ncurses a pameti
    refresh(); // zobrazeni okna podle toho co je v pameti
    printw("Hello world!"); // vypis textu

    getch(); // cekani na stisk klavesy
    endwin(); // dealoc pameti a zavreni ncurses

    return 0;
}
