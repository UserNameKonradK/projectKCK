#ifndef MAGIC_TILES_MODEL_H
#define MAGIC_TILES_MODEL_H

#include <vector>
#include <deque>
#include <utility>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <curses.h>

// Constants
#define WIDTH_M 120
#define HEIGHT_M 30
#define WIDTH 30
#define HEIGHT 25
#define NUM_TRACKS 4
#define TILE_WIDTH 6
#define TILE_HEIGHT 7
#define START_Y -1
#define MAX_TILES_PER_LINE 2

using namespace std;
using namespace std::chrono;

// Struct representing a tile in the game
struct Tile {
    int x, y;           // Position of the tile
    int track;          // Track number the tile belongs to
    bool isActive;      // Whether the tile is active
};

// MagicTilesModel class: Handles game logic and data
class MagicTilesModel {
public:
    MagicTilesModel(int f_choice); // Constructor

    // Getters
    int getAllTiles();
    int getNumOfTiles();
    int getNumOfDelTiles();
    int getStrick();
    int getScore();

    // Core game functions
    void readFile();            // Load tile data from a file
    void resetStartTime();      // Reset the game timer
    void updateGameLogic();     // Update game logic (e.g., moving tiles, spawning new tiles)
    void handleTileHit(int track); // Handle player hitting a tile
    void initializeTiles();     // Initialize tiles on tracks

    // Public member variables
    int file_choice;
    bool gameOver;              // Whether the game is over
    int score;                  // Player's score
    int lives;                  // Remaining lives
    int allTiles;               // Total number of tiles
    int numOfTiles;             // Number of active tiles
    int numOfDelTiles;          // Number of deleted tiles
    int strick, maxStrick;      // Current and maximum streak of hits
    int tilesTaken;             // Count of tiles successfully hit
    Tile tiles[NUM_TRACKS][MAX_TILES_PER_LINE]; // Array of tiles for all tracks
    deque<pair<int, int>> Tiles;               // Tile generation data from file
    size_t currentIndex;                       // Current tile index for spawning
    steady_clock::time_point startTime;        // Start time for game logic

private:
    

    // Helper functions
    void spawnTile(int track);                 // Spawn a new tile in the given track
    void deleteTile(const Tile &tile);         // Delete a tile from the game
    void updateScore();                        // Update the score based on hits
};

#endif // MAGIC_TILES_MODEL_H
