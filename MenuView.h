#ifndef MENU_VIEW_H
#define MENU_VIEW_H

#include <ncurses.h>
#include "MenuModel.h"  // Include MenuModel header for accessing menu data

#define WIDTH_M 120
#define HEIGHT_M 30

extern const char *title[];

extern const char *choices_menu[]; // Declares the menu choices


class MenuView {
    WINDOW *menu_win;

public:
    // Constructor to initialize ncurses and create a window for the menu
    MenuView();

    int color_swith;

    // Destructor to clean up the menu window and end ncurses mode
    ~MenuView();

    void color_select();

    int getColor_swith();

    // Prints the title of the menu
    void print_title(WINDOW *menu_win, int tx, int ty);

    // Prints the menu options with highlighting for the selected choice
    void print_menu(int highlight, MenuModel &model);

    // Displays a screen when the user exits the game
    void exitScreen();

    // Returns the menu window object for further manipulation
    WINDOW *get_window();
};

#endif // MENU_VIEW_H
