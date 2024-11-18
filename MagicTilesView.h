#ifndef MAGIC_TILES_VIEW_H
#define MAGIC_TILES_VIEW_H

#include <curses.h>
#include "MagicTilesModel.h"

// Constants for rendering
#define TILE_CHAR ' '
#define TILE_WIDTH 6
#define TILE_HEIGHT 7
#define START_Y -1
#define NUM_TRACKS 4
#define MAX_TILES_PER_LINE 2

// MagicTilesView class: Handles the game's visual representation
class MagicTilesView {
public:
    int color;
    // Constructor: Initializes the view with a reference to the model
    MagicTilesView(MagicTilesModel &m, int color);

    // Destructor: Cleans up the screen and prints a summary
    ~MagicTilesView();

    // Draws the current state of the game
    void drawGame(const MagicTilesModel &model);

    // Draws the game-over screen
    void drawGameOver(const MagicTilesModel &model);

private:
    MagicTilesModel &model; // Reference to the game's model

    // Draws a single tile on the screen
    void drawTile(const Tile &tile);

    // Draws the user interface (score, controls, etc.)
    void drawUI(const MagicTilesModel &model);
};

#endif // MAGIC_TILES_VIEW_H
