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

#include "MagicTilesModel.h"
#include "MagicTilesView.h"
#include "MagicTilesController.h"
#include "FileModel.h"
#include "FileView.h"
#include "FileController.h"
#include "MenuModel.h"
#include "MenuView.h"
#include "MenuController.h"


using namespace std;
using namespace std::chrono;

    MenuController::MenuController(MenuView &view, MenuModel &model) : view(view), model(model), highlight(1), choice(0), file(0)/*, 
    gameModel(), gameView(gameModel), gameController(gameModel, gameView)*/ {}

    void MenuController::select_option() {
        WINDOW *menu_win = view.get_window();
        while (true) {
            usleep(50000);
            if (choice != 1 /*&& choice != 4*/)
                c = wgetch(menu_win);
            //nodelay(menu_win, FALSE);
            switch (c) {
                case KEY_UP:
                    highlight = (highlight == 1) ? model.visible_choices : highlight - 1;
                    break;
                case KEY_DOWN:
                    highlight = (highlight == model.visible_choices) ? 1 : highlight + 1;
                    break;
                case 10: // Enter key
                    choice = highlight;
                    //return choice;
                    break;
                default:
                    break;
            }
            
            if (choice == 1 || choice == 4) {
                if (choice == 1) {
                    FileModel fileModel;
                    FileView fileView(view.getColor_swith()+1);
                    FileController fileController(fileView, fileModel);
                    fileView.file_print_menu(1, fileModel);
                    file = fileController.file_select_option();
                }
                if (file != 4 || choice == 4) {
                    MagicTilesModel gameModel(file);
                    MagicTilesView gameView(gameModel, view.getColor_swith()+1);
                    MagicTilesController gameController(gameModel, gameView);
                    choice = gameController.run();  // Uruchomienie gry
                }
                else
                    choice = 0;
                
                //view.print_menu(highlight, model);
            } 
            else if (choice == 2) {
                // Obsługa opcji 2
                view.color_select();
                choice = 0;  // Resetowanie wyboru
            } 
            else if (choice == 3) {
                // Wyjście z menu
                view.exitScreen();
                getch();
                break;  // Wyjście z pętli
            }
            if (choice == 0) view.print_menu(highlight, model);
        }
    }
