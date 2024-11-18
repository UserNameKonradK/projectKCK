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
#include "ResultModel.h"
#include "ResultView.h"
#include "ResultController.h"


using namespace std;
using namespace std::chrono;

    ResultController::ResultController(ResultView &resultView, ResultModel &resultModel, MagicTilesModel &gameModel) : resultView(resultView), resultModel(resultModel), highlight(1), choice(0), 
    gameModel(gameModel)/*, gameView(gameModel), gameController(gameModel, gameView)*/ {}

    int ResultController::select_option() {
        WINDOW *result_win = resultView.get_window();
        //choice = 0;
        while (true) {
            int c = wgetch(result_win);
            switch (c) {
                case KEY_UP:
                    highlight = (highlight == 1) ? resultModel.visible_choices_result : highlight - 1;
                    break;
                case KEY_DOWN:
                    highlight = (highlight == resultModel.visible_choices_result) ? 1 : highlight + 1;
                    break;
                case 10: // Enter key
                    choice = highlight;
                    //return choice;
                    break;
                default:
                    break;
            }
            resultView.print_menu(highlight, resultModel, gameModel);
            if (choice == 1) {
                choice = 4;
                break;
            } 
            else if (choice == 2) {
                choice = 1;
                break;
            }
            else if (choice == 3) {
                choice = 0;
                break;
            }
        }
        return choice;
    }