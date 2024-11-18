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

#include "FileModel.h"  // Include ResultModel header to use ResultModel
#include "FileView.h"


#define WIDTH_M 120
#define HEIGHT_M 30

using namespace std;
using namespace std::chrono;

const char *file_title[] = {
"  SSS   EEEEE  L      EEEEE  CCCCC  TTTTT     FFFFF  III  L      EEEEE",
" S      E      L      E      C        T       F       I   L      E",
"  SSS   EEEE   L      EEEE   C        T       FFFF    I   L      EEEE",
"     S  E      L      E      C        T       F       I   L      E",
"  SSS   EEEEE  LLLLL  EEEEE  CCCCC    T       F      III  LLLLL  EEEEE",

};

const char *choices_file[] = {
    "   0000000000    ",
    "    Easy file    ",
    "   0000000000    ",
    "   0000000000    ",
    "   Normal file   ",
    "   0000000000    ",
    "   0000000000    ",
    "    HARD FILE    ",
    "   0000000000    ",
    "   0000000000    ",
    "  Back to menu   ",
    "   0000000000    ",
};


    FileView::FileView(int colorr) {
        color = colorr;
        // initscr();
        // start_color();
        // clear();
        // noecho();
        // curs_set(0);
        // cbreak();
        init_pair(1, COLOR_RED, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_BLUE);
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
        result_win = newwin(HEIGHT_M, WIDTH_M, 0, 0);
        keypad(result_win, TRUE);
        refresh();
    }

    FileView::~FileView() {
        delwin(result_win);
       // endwin();
    }

    void FileView::file_print_title(WINDOW *result_win, int tx, int ty) {
        int i, j;
        const char *line;
    
        for (i = 0; i < 5; i++) {  // 5 linii tytułu
            line = file_title[i];
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

    void FileView::file_print_menu(int highlight, FileModel &fileModel) {
        int x = WIDTH_M / 2 - 10;
        int y = HEIGHT_M / 2 - fileModel.visible_choices_result_y / 2 + 1;
        int tx = WIDTH_M / 2 - 36;
        int ty = HEIGHT_M / 2 - 10;

        wbkgd(result_win, COLOR_PAIR(color));
        box(result_win, 0, 0);
        file_print_title(result_win, tx, ty);

        for (int i = 0; i <fileModel.visible_choices_result_y; ++i) {
            int index = i * 3 + 1; // Main menu options are indexed by this
            if (highlight == i + 1) {  // Podświetlenie
                wattron(result_win, COLOR_PAIR(color) | A_REVERSE | A_INVIS);
                mvwprintw(result_win, y-1, x, "%-10s", choices_file[index-1]);
                wattroff(result_win, A_INVIS);
                mvwprintw(result_win, y, x, "%-10s", choices_file[index]);  // Cały "przycisk"
                wattron(result_win, A_INVIS);
                mvwprintw(result_win, y+1, x, "%-10s", choices_file[index+1]);
                wattroff(result_win, COLOR_PAIR(color) | A_REVERSE | A_INVIS);
            } else {
                wattron(result_win, A_INVIS);
                mvwprintw(result_win, y-1, x, "%-10s", choices_file[index-1]);
                wattroff(result_win, A_INVIS);
                mvwprintw(result_win, y, x, "%-10s", choices_file[index]);  // Niepodświetlony
                wattron(result_win, A_INVIS);
                mvwprintw(result_win, y+1, x, "%-10s", choices_file[index+1]);
                wattroff(result_win, A_INVIS);
            }
            y += 3; // Move to next option row
        }
        wrefresh(result_win);
    }

    WINDOW *FileView::get_window() { return result_win; }
