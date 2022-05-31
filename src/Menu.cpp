#include "Menu.h"


CMenu::CMenu(std::vector<std::string> text, std::vector<std::string> options)
{
    _text = std::move(text);
    _options = std::move(options);
}

int newMenu(const CMenu & menu, bool wait){
    terminal term;
    int height = 25;
    int width = 75;
    int posX = (width/2) - (int)(menu._text.size()/2) - 10;

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
    int keypress = 0;
    int choice = 0;
    if(wait) {
        posY++;
        mvwprintw(menu_win, posY, posX, "Press any key to continue");
        printShrek(menu_win, posY+1,posX);
        refresh();
        wgetch(menu_win);
        mvwprintw(menu_win, posY, posX, "                         ");
        clearShrek(menu_win, posY+1,posX);
    }
    posY++;
    int temp = posY;
    while(true){
        posY = temp;

        if (keypress == KEY_UP) {
            choice = (choice - 1) ;
            if(choice == -1)
                choice =  (int)menu._options.size() - 1;
            else
                choice %= (int)menu._options.size();
        } else if (keypress == KEY_DOWN) {
            choice = (choice + 1) % (int)menu._options.size();
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
        keypress = wgetch(menu_win);
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
                         }), true);
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
