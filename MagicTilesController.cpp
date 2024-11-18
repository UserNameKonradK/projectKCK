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
#include "MagicTilesView.h"
#include "MagicTilesController.h"

using namespace std;
using namespace std::chrono;


    MagicTilesController::MagicTilesController(MagicTilesModel &gameModel, MagicTilesView &gameView)
        : gameModel(gameModel), gameView(gameView) {}

    int MagicTilesController::run() {
        timeout(0);
        while (!gameModel.gameOver) {
            gameView.drawGame(gameModel);
            processInput();
            gameModel.updateGameLogic();
            usleep(50000);
        }
        clear();
        //gameView.drawGameOver(gameModel);
        int choice;
        ResultModel resultModel;
        ResultView resultView(gameView.color);
        ResultController resultController(resultView, resultModel, gameModel);
        resultView.print_menu(1, resultModel, gameModel);
        choice = resultController.select_option();
        return choice;
    }

    void MagicTilesController::processInput() {
        int ch = getch();
        switch (ch) {
            case 'h': gameModel.handleTileHit(0); break;
            case 'j': gameModel.handleTileHit(1); break;
            case 'k': gameModel.handleTileHit(2); break;
            case 'l': gameModel.handleTileHit(3); break;
            case 'q': gameModel.gameOver = true; break;
            default: break;
        }
    }