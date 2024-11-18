#include <iostream>
#include <curses.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <vector>
#include <utility>
#include <fstream>
#include <chrono> 
#include <thread>
#include <deque>

#include "MenuModel.h"
#include "MenuView.h" 

#define WIDTH_M 120
#define HEIGHT_M 30

using namespace std;
using namespace std::chrono;

// Global title and menu data
const char *title[] = {
    "M   M   A    GGG  III  CCCC    TTTTT III  L     EEEEE  SSS ",
    "MM MM  A A  G      I  C          T    I   L     E     S    ",
    "M M M A   A G  GG  I  C          T    I   L     EEEE   SSS ",
    "M   M AAAAA G   G  I  C          T    I   L     E         S",
    "M   M A   A  GGG  III  CCCC      T   III  LLLLL EEEEE  SSS" ,
    "                                       Deluxen't Edition"
};

const char *choices_menu[] = {
    "   0000000000   ",
    "      Play      ",
    "   0000000000   ",
    "   0000000000   ",
    "  Change theme  ",
    "   0000000000   ",
    "   0000000000   ",
    "      Exit      ",
    "   0000000000   ",
};

    MenuView::MenuView() {
        initscr();
        start_color();
        clear();
        noecho();
        curs_set(0);
        cbreak();
        menu_win = newwin(HEIGHT_M, WIDTH_M, 0, 0);
        keypad(menu_win, TRUE);
        refresh();
        init_pair(1, COLOR_RED, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_BLUE);
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
        color_swith = 0;
    }

    MenuView::~MenuView() {
        delwin(menu_win);
        endwin();
    }

    void MenuView::color_select() {
        if (color_swith == 0)
            attron(COLOR_PAIR(1));
        else if (color_swith == 1)
            attron(COLOR_PAIR(2));
        else if (color_swith == 1)
            attron(COLOR_PAIR(3));
        else
            attron(COLOR_PAIR(4));
        color_swith = (color_swith + 1) % 4;
    }

    int MenuView::getColor_swith() {
        return color_swith;
    }

    void MenuView::print_title(WINDOW *menu_win, int tx, int ty) {
        int i, j;
        const char *line;
    
        for (i = 0; i < 5; i++) {  // 5 linii tytułu
            line = title[i];
            for (j = 0; line[j] != '\0'; j++) {
                if (line[j] != ' ') {  // Sprawdzanie, czy znak nie jest spacją
                    wattron(menu_win, COLOR_PAIR((color_swith % 4) + 1) | A_REVERSE | A_INVIS);  // Podświetlanie
                    mvwaddch(menu_win, ty + i, tx + j, line[j]);
                    wattroff(menu_win, COLOR_PAIR((color_swith % 4) + 1) | A_REVERSE | A_INVIS);  // Wyłączanie podświetlenia
                } else {
                    mvwaddch(menu_win, ty + i, tx + j, line[j]);  // Zwykły wydruk dla spacji
                }
            }
        }
        mvwprintw(menu_win, ty+5, tx, "%-64s", title[5]);
        wrefresh(menu_win);  // Odświeżenie okna, aby zmiany były widoczne
    }

    void MenuView::print_menu(int highlight, MenuModel &model) {
        int x = WIDTH_M / 2 - 10;
        int y = HEIGHT_M / 2 - model.visible_choices / 2 + 2;
        int tx = WIDTH_M / 2 - 30;
        int ty = HEIGHT_M / 2 - 10;

        wbkgd(menu_win, COLOR_PAIR((color_swith % 4) + 1));
        box(menu_win, 0, 0);
        print_title(menu_win, tx, ty);

        for (int i = 0; i < model.visible_choices; ++i) {
            int index = i * 3 + 1; // Main menu options are indexed by this
            if (highlight == i + 1) {  // Podświetlenie
                wattron(menu_win, COLOR_PAIR((color_swith % 4) + 1) | A_REVERSE | A_INVIS);
                mvwprintw(menu_win, y-1, x, "%s", choices_menu[index-1]);
                wattroff(menu_win, A_INVIS);
                mvwprintw(menu_win, y, x, "%s", choices_menu[index]);  // Cały "przycisk"
                wattron(menu_win, A_INVIS);
                mvwprintw(menu_win, y+1, x, "%s", choices_menu[index+1]);
                wattroff(menu_win, COLOR_PAIR((color_swith % 4) + 1) | A_REVERSE | A_INVIS);
            } else {
                wattron(menu_win, A_INVIS);
                mvwprintw(menu_win, y-1, x, "%s", choices_menu[index-1]);
                wattroff(menu_win, A_INVIS);
                mvwprintw(menu_win, y, x, "%s", choices_menu[index]);  // Niepodświetlony
                wattron(menu_win, A_INVIS);
                mvwprintw(menu_win, y+1, x, "%s", choices_menu[index+1]);
                wattroff(menu_win, A_INVIS);
            }
            y += 3; // Move to next option row
        }
        wrefresh(menu_win);
    }

    void MenuView::exitScreen () {
        clear();
        mvprintw(HEIGHT_M / 2, WIDTH_M / 2 - 8, "Thank you for your attention!");
    }

    WINDOW *MenuView::get_window() { return menu_win; }
