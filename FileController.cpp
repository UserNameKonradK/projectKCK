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

#include "FileModel.h"
#include "FileView.h"
#include "FileController.h"


using namespace std;
using namespace std::chrono;

    FileController::FileController(FileView &fileView, FileModel &fileModel) : fileView(fileView), fileModel(fileModel), highlight(1), choice(0) 
    /*, gameView(gameModel), gameController(gameModel, gameView)*/ {}

    int FileController::file_select_option() {
        WINDOW *result_win = fileView.get_window();
        //choice = 0;
        while (true) {
            int c = wgetch(result_win);
            switch (c) {
                case KEY_UP:
                    highlight = (highlight == 1) ? fileModel.visible_choices_result_y : highlight - 1;
                    break;
                case KEY_DOWN:
                    highlight = (highlight == fileModel.visible_choices_result_y) ? 1 : highlight + 1;
                    break;
                // case KEY_LEFT:
                //     highlight = (highlight == 1) ? fileModel.visible_choices_result_x : highlight - 1;
                //     break;
                // case KEY_RIGHT:
                //     highlight = (highlight == fileModel.visible_choices_result_x) ? 1 : highlight + 1;
                //     break;
                case 10: // Enter key
                    choice = highlight;
                    //return choice;
                    break;
                default:
                    break;
            }
            fileView.file_print_menu(highlight, fileModel);
            if (choice == 1)  // Wybrano opcję
                break;
            else if (choice == 2) 
                break;
            else if (choice == 3) 
                break;
            else if (choice == 4) {
                //choice = 0;
                break;
            }
                
        }
        return choice;
    }