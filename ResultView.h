#ifndef RESULT_VIEW_H
#define RESULT_VIEW_H

#include <curses.h>
#include "MagicTilesModel.h"  // Include MagicTilesModel for game-related data
#include "ResultModel.h"  // Include ResultModel header to use ResultModel


#define WIDTH_M 120
#define HEIGHT_M 30

extern const char *result[];  // Declaration of the result title strings
// extern const char *choices_result[];  // Declaration of the result menu options
extern const char *choices_result[];  // Declaration of the result menu options

class ResultView {
    WINDOW *result_win;  // Pointer to the curses window

public:
    int color;
    // Constructor initializes the curses window
    ResultView(int colorr);

    // Destructor cleans up the window
    ~ResultView();

    // Function to print the title in the result window
    void print_title(WINDOW *result_win, int tx, int ty);

    // Function to print the result menu with highlight and choices
    void print_menu(int highlight, ResultModel &resultModel, MagicTilesModel &gameModel);

    // Getter for the result window
    WINDOW *get_window();
};

#endif // RESULT_VIEW_H
