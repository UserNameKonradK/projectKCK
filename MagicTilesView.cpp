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


#define WIDTH 30
#define HEIGHT 25
#define NUM_TRACKS 4 //8
#define TILE_CHAR ' '
#define TILE_WIDTH 6
#define TILE_HEIGHT 7
#define START_Y -1
#define MAX_TILES_PER_LINE 2

using namespace std;
using namespace std::chrono;


    MagicTilesView::MagicTilesView(MagicTilesModel &m, int colorr) : model(m) {
        color = colorr;
        init_pair(1, COLOR_RED, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_BLUE);
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
        init_pair(1 + 4, COLOR_RED, COLOR_RED);
        init_pair(2 + 4, COLOR_WHITE, COLOR_WHITE);
        init_pair(3 + 4, COLOR_WHITE, COLOR_WHITE);
        init_pair(4 + 4, COLOR_YELLOW, COLOR_YELLOW);
        bkgd(COLOR_PAIR(color));
    }

    MagicTilesView::~MagicTilesView() { 
        clear();
        //endwin(); 
        //printf("allTiles: %d, numOfTiles: %d, numOfDelTiles: %d\tSeria: %d Wynik: %d\n", model.getAllTiles(), model.getNumOfTiles(), model.getNumOfDelTiles(), model.getStrick(), model.getScore());
    }

    void MagicTilesView::drawGame(const MagicTilesModel &model) {
        for (int i = 0; i < NUM_TRACKS; ++i) {
            for (int j = 0; j < MAX_TILES_PER_LINE; j++){
                if (model.tiles[i][j].isActive) {
                    drawTile(model.tiles[i][j]); // Rysuj aktywny kafelek
                }
            }    
        }
        drawUI(model);
        refresh();
    }

    void MagicTilesView::drawGameOver(const MagicTilesModel &model) {
        //mvprintw(10, 10, "Game Over! Final Score: %d", model.score);
        refresh();
    }

    // void eraseTile(const Tile &tile) {
    //     for (int i = -1; i < TILE_HEIGHT; ++i) {
    //         for (int j = 0; j < TILE_WIDTH; ++j) {
    //             mvaddch(tile.y + i, tile.x + j, ' '); // Czyszczenie obszaru kafelka
    //         }
    //     }
    // }


    void MagicTilesView::drawTile(const Tile &tile) {
        if (tile.y > START_Y) {
            for (int j = 0; j < TILE_WIDTH; ++j) {
                mvaddch(tile.y - 1, tile.x + j, ' ');  // Czyszczenie poprzedniej gďż˝rnej linii kafelka

            }
        }

        // Rysowanie dolnej linii kafelka
        for (int j = 0; j < TILE_WIDTH; ++j) {
            attron(COLOR_PAIR(color + 4));
            mvaddch(tile.y + TILE_HEIGHT - 1, tile.x + j, TILE_CHAR);  // Rysowanie tylko dolnej linii kafelka
            attroff(COLOR_PAIR(color+ 4));
        }
    }

    void MagicTilesView::drawUI(const MagicTilesModel &model) {
        mvprintw(1, 1, "Score: %d", model.score);
        mvprintw(2, 1, "Lives: %d", model.lives);
        mvprintw(3, 1, "Series: %d   ", model.strick);
        mvprintw(1, 34, "Controls: h - track 1, j - track 2, k - track 3, l - track 4");
        mvprintw(2, 52, "Press q to exit");

        int beamY = HEIGHT - 1;
        attron(COLOR_PAIR(color + 4));
        mvhline(beamY, 0, ' ', 120);
        mvhline(29, 0, ' ', 119);
        mvhline(0, 0, ' ', 119);
        mvhline(4, 0, ' ', 119);
        mvvline(0 , 119, ' ', 30);
        mvvline(0 , 0, ' ', 30);
        attroff(COLOR_PAIR(color + 4));
    }