#ifndef MAGIC_TILES_CONTROLLER_H
#define MAGIC_TILES_CONTROLLER_H

#include "MagicTilesModel.h"
#include "ResultModel.h"
#include "ResultView.h"
#include "ResultController.h"
#include "MagicTilesView.h"

// MagicTilesController class: Manages the game's main loop and user input
class MagicTilesController {
public:
    // Constructor: Initialize the controller with references to the model and view
    MagicTilesController(MagicTilesModel &gameModel, MagicTilesView &gameView);

    // Runs the main game loop and handles game flow; returns the user's menu choice after the game ends
    int run();

private:
    MagicTilesModel &gameModel; // Reference to the game's model
    MagicTilesView &gameView;   // Reference to the game's view

    // Processes user input during the game
    void processInput();
};

#endif // MAGIC_TILES_CONTROLLER_H
