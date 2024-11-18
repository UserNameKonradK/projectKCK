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

#include "MagicTilesModel.h"  // Include MagicTilesModel for game-related data
#include "ResultModel.h"  // Include ResultModel header to use ResultModel
#include "ResultView.h"


#define WIDTH_M 120
#define HEIGHT_M 30

using namespace std;
using namespace std::chrono;

const char *result[] = {
    "RRRRR  EEEEE  SSS   U   U  L    TTTTT",
    "R   R  E     S      U   U  L      T  ",
    "RRRRR  EEEE   SSS   U   U  L      T  ",
    "R  R   E         S  U   U  L      T  ",
    "R   R  EEEEE  SSS    UUU   LLLLL  T  ",
};

const char *choices_result[] = {
    "   0000000000   ",
    "   Play again   ",
    "   0000000000   ",
    "   0000000000   ",
    "   Change map   ",
    "   0000000000   ",
    "   0000000000   ",
    "  Back to menu  ",
    "   0000000000   ",
};

    ResultView::ResultView(int colorr) {
        color = colorr;
        // initscr();
        // start_color();
        // clear();
        // noecho();
        // curs_set(0);
        // cbreak();
        result_win = newwin(HEIGHT_M, WIDTH_M, 0, 0);
        keypad(result_win, TRUE);
        refresh();
    }

    ResultView::~ResultView() {
        delwin(result_win);
       // endwin();
    }

    void ResultView::print_title(WINDOW *result_win, int tx, int ty) {
        int i, j;
        const char *line;
    
        for (i = 0; i < 5; i++) {  // 5 linii tytułu
            line = result[i];
            for (j = 0; line[j] != '\0'; j++) {
                if (line[j] != ' ') {  // Sprawdzanie, czy znak nie jest spacją
                    wattron(result_win, COLOR_PAIR(color) | A_REVERSE | A_INVIS);  // Podświetlanie
                    mvwaddch(result_win, ty + i, tx + j, line[j]);
                    wattroff(result_win, COLOR_PAIR(color) | A_REVERSE | A_INVIS);  // Wyłączanie podświetlenia
                } else {
                    mvwaddch(result_win, ty + i, tx + j, line[j]);  // Zwykły wydruk dla spacji
                }
            }
        }
        //mvwprintw(result_win, ty+5, tx, "%-64s", result[5]);
        wrefresh(result_win);  // Odświeżenie okna, aby zmiany były widoczne
    }

    void ResultView::print_menu(int highlight, ResultModel &resultModel, MagicTilesModel &gameModel) {
        int x = WIDTH_M / 2 - 10;
        int y = HEIGHT_M / 2 - resultModel.visible_choices_result / 2 + 2;
        int tx = WIDTH_M / 2 - 20;
        int ty = HEIGHT_M / 2 - 10;

        wbkgd(result_win, COLOR_PAIR(color));
        box(result_win, 0, 0);
        print_title(result_win, tx, ty);
        mvwprintw(result_win, ty+6, tx - 6, "Score: %d", gameModel.score);
        mvwprintw(result_win, ty+6, tx + 11, "Acurccy: %.2f%% ",(float)gameModel.tilesTaken/gameModel.getNumOfDelTiles() * 100);
        mvwprintw(result_win, ty+6, tx + 33, "Highest series: %d", gameModel.maxStrick);

        for (int i = 0; i <resultModel.visible_choices_result; ++i) {
            int index = i * 3 + 1; // Main menu options are indexed by this
            if (highlight == i + 1) {  // Podświetlenie
                wattron(result_win, COLOR_PAIR(color) | A_REVERSE | A_INVIS);
                mvwprintw(result_win, y-1, x, "%-10s", choices_result[index-1]);
                wattroff(result_win, A_INVIS);
                mvwprintw(result_win, y, x, "%-10s", choices_result[index]);  // Cały "przycisk"
                wattron(result_win, A_INVIS);
                mvwprintw(result_win, y+1, x, "%-10s", choices_result[index+1]);
                wattroff(result_win, COLOR_PAIR(color) | A_REVERSE | A_INVIS);
            } else {
                wattron(result_win, A_INVIS);
                mvwprintw(result_win, y-1, x, "%-10s", choices_result[index-1]);
                wattroff(result_win, A_INVIS);
                mvwprintw(result_win, y, x, "%-10s", choices_result[index]);  // Niepodświetlony
                wattron(result_win, A_INVIS);
                mvwprintw(result_win, y+1, x, "%-10s", choices_result[index+1]);
                wattroff(result_win, A_INVIS);
            }
            y += 3; // Move to next option row
        }
        wrefresh(result_win);
    }

    WINDOW *ResultView::get_window() { return result_win; }
