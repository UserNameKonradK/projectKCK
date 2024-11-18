#ifndef FILE_VIEW_H
#define FILE_VIEW_H

#include <curses.h>
#include "FileModel.h"  // Include ResultModel header to use ResultModel


#define WIDTH_M 120
#define HEIGHT_M 30

extern const char *file_title[];  // Declaration of the result title strings
// extern const char *choices_result[];  // Declaration of the result menu options
extern const char *choices_file[];  // Declaration of the result menu options

class FileView {
    WINDOW *result_win;  // Pointer to the curses window

public:
    int color;
    // Constructor initializes the curses window
    FileView(int colorr);

    // Destructor cleans up the window
    ~FileView();

    // Function to print the title in the result window
    void file_print_title(WINDOW *result_win, int tx, int ty);

    // Function to print the result menu with highlight and choices
    void file_print_menu(int highlight, FileModel &fileModel);

    // Getter for the result window
    WINDOW *get_window();
};

#endif // FILE_VIEW_H
