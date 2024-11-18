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

#define WIDTH_M 120
#define HEIGHT_M 30
#define WIDTH 30
#define HEIGHT 25
#define NUM_TRACKS 4 //8
#define TILE_WIDTH 6
#define TILE_HEIGHT 7
#define START_Y -1
#define MAX_TILES_PER_LINE 2

using namespace std;
using namespace std::chrono;

    MagicTilesModel::MagicTilesModel(int f_choice) {
        file_choice = f_choice;
        gameOver = false;       // Stan gry
        score = 0;
        lives = 3;               // Wynik gracza
        allTiles = 100;
        numOfTiles = 0;
        numOfDelTiles = 0;
        strick = 0, maxStrick = 0, tilesTaken = 0;
        currentIndex = 0;
        readFile();
        resetStartTime();
        initializeTiles();
    }

    int MagicTilesModel::getAllTiles() {
        return allTiles;
    }
    int MagicTilesModel::getNumOfTiles() {
        return numOfTiles;
    }
    int MagicTilesModel::getNumOfDelTiles() {
        if (numOfDelTiles == 0)
            numOfDelTiles = 1;
        return numOfDelTiles;
    }
    int MagicTilesModel::getStrick() {
        return strick;
    }
    int MagicTilesModel::getScore() {
        return score;
    }

    void MagicTilesModel::readFile(){
        ifstream file;
        if (file_choice == 1)
             file.open("map1.txt");
        else if (file_choice == 1)
             file.open("map2.txt");
        else
             file.open("map3.txt");
        //ifstream file("map1.txt");  // Otwórz plik do odczytu
            if (!file) {
                cout << "Nie udało się otworzyć pliku!" << std::endl;
                exit(EXIT_FAILURE);
            }

        int track, apptime;
        while (file >> track >> apptime) {
            Tiles.push_back(make_pair(track, apptime));  // Dodaj parę do wektora
            //cout << "Wczytano: track = " << track << ", apptime = " << apptime << endl;
        }

        file.close();
    }

    void MagicTilesModel::resetStartTime() {
        startTime = std::chrono::steady_clock::now(); // Resetowanie czasu
    }

    void MagicTilesModel::updateGameLogic() {
        for (int i = 0; i < NUM_TRACKS; ++i) {
            for (int j = 0; j < MAX_TILES_PER_LINE; j++){
                if (tiles[i][j].isActive) {
                    tiles[i][j].y++;
                    if (tiles[i][j].y + TILE_HEIGHT - 1 >= HEIGHT + TILE_HEIGHT) {
                    // Kafelek dotarďż˝ na dďż˝ i nie zostaďż˝ strďż˝cony
                        lives--;  // Utrata punktu
                        if (maxStrick < strick) {
                            maxStrick = strick;
                        }
                        strick = 0;
                        tiles[i][j].isActive = false;
                        //view.eraseTile();
                        deleteTile(tiles[i][j]);
                    }
                }
            }
        }
        //static auto startTime = steady_clock::now();
        auto currentTime = steady_clock::now();
        auto elapsedTime = duration_cast<milliseconds>(currentTime - startTime).count();
    

        if (currentIndex < Tiles.size() && elapsedTime >= Tiles[currentIndex].second) {
            int track = Tiles[currentIndex].first;  // Get the track of the tile
            spawnTile(track);                      // Create the tile on the corresponding track
            currentIndex++;                        // Move to the next tile
        }
        if (lives <= 0) {
            //gameOver = true;
        }
    }

    void MagicTilesModel::handleTileHit(int track) {
        if (tiles[track][0].track == track && tiles[track][0].isActive) { 
            if (tiles[track][0].y + TILE_HEIGHT - 1 >= HEIGHT - 1) {
                // Gracz zbiďż˝ kafelek - dodanie punktu
                updateScore();

                tiles[track][0].isActive = false; // Deaktywacja kafelka po zbiciu
                //view.eraseTile();
                deleteTile(tiles[track][0]);
            } 
            else if (tiles[track][1].track == track && tiles[track][1].isActive) {
                if (tiles[track][1].y + TILE_HEIGHT - 1 >= HEIGHT - 1) {
                    // Gracz zbiďż˝ kafelek - dodanie punktu
                    updateScore();

                    tiles[track][1].isActive = false; // Deaktywacja kafelka po zbiciu
                    //view.eraseTile();
                    deleteTile(tiles[track][0]);
                }
            } else {
                if (maxStrick < strick) {
                    maxStrick = strick;
                }
                strick = 0;
                lives--;
            } 
        }
    }

    void MagicTilesModel::initializeTiles() {
        for (int i = 0; i < NUM_TRACKS; ++i) {
            for (int j = 0; j < MAX_TILES_PER_LINE; j++) {
                tiles[i][j] = {0, START_Y, i, false};
            }
        }
    }

    void MagicTilesModel::spawnTile(int tracks) {
        for (int j = 0; j < MAX_TILES_PER_LINE; j++) {
            if (!tiles[tracks][j].isActive) {
                tiles[tracks][j].track = tracks;     // Przypisanie toru
                tiles[tracks][j].x = tracks * (TILE_WIDTH + 1) + 47; // Ustawienie pozycji X w zaleďż˝noďż˝ci od toru
                tiles[tracks][j].y = START_Y;   // Poczďż˝tek od gďż˝ry
                tiles[tracks][j].isActive = true; // Kafelek staje siďż˝ aktywny
                numOfTiles++;
                break;
            }
        }
    }

    void MagicTilesModel::deleteTile(const Tile &tile) {
        for (int i = -1; i < TILE_HEIGHT; ++i) {
            for (int j = 0; j < TILE_WIDTH; ++j) {
                mvaddch(tile.y + i, tile.x + j, ' '); // Czyszczenie obszaru kafelka
            }
        }
        if (numOfDelTiles < allTiles) numOfDelTiles++;
        if (numOfDelTiles == allTiles) {
            if (maxStrick < strick) {
                    maxStrick = strick;
                }
            gameOver = true;
        }
            
    }

    void MagicTilesModel::updateScore() {
        tilesTaken++;
        if (strick < 10) {
            score++; strick++;
        } else if (strick >= 10 && strick < 20) {
            score += 2; strick++;
        } else if (strick >= 20 && strick < 30) {
            score += 5; strick++;
        } else if (strick >= 30 && strick < 50) {
            score += 10; strick++;
        } else {
            score += 20; strick++;
        }
    }
