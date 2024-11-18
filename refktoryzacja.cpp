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

#define WIDTH_M 120
#define HEIGHT_M 30
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

// Global title and menu data
const char *title[] = {
    "M   M   A    GGG  III  CCCC    TTTTT III  L     EEEEE  SSS ",
    "MM MM  A A  G      I  C          T    I   L     E     S    ",
    "M M M A   A G  GG  I  C          T    I   L     EEEE   SSS ",
    "M   M AAAAA G   G  I  C          T    I   L     E         S",
    "M   M A   A  GGG  III  CCCC      T   III  LLLLL EEEEE  SSS" ,
    "                                       Deluxen't Edition"
};

const char *choices_menu[] = {
    "   0000000000   ",
    "      Play      ",
    "   0000000000   ",
    "   0000000000   ",
    "     Options    ",
    "   0000000000   ",
    "   0000000000   ",
    "      Exit      ",
    "   0000000000   ",
};

const char *result[] = {
    "RRRRR  EEEEE  SSS   U   U  L    TTTTT",
    "R   R  E     S      U   U  L      T  ",
    "RRRRR  EEEE   SSS   U   U  L      T  ",
    "R  R   E         S  U   U  L      T  ",
    "R   R  EEEEE  SSS    UUU   LLLLL  T  ",
};

const char *choices_result[] = {
    "   0000000000   ",
    "   Play again   ",
    "   0000000000   ",
    "   0000000000   ",
    "   Change map   ",
    "   0000000000   ",
    "   0000000000   ",
    "  Back to menu  ",
    "   0000000000   ",
};

struct Tile {
    int x, y;
    int track;
    bool isActive;
};



class MagicTilesModel {
public:
    Tile tiles[NUM_TRACKS][MAX_TILES_PER_LINE];      // Tablica kafelkďż˝w dla czterech torďż˝w
    bool gameOver = false;       // Stan gry
    int score = 0;
    int lives = 3;               // Wynik gracza
    int allTiles = 100;
    int numOfTiles = 0;
    int numOfDelTiles = 0;
    int strick = 0, maxStrick = 0, tilesTaken = 0;
    size_t currentIndex = 0;
    steady_clock::time_point startTime;
    

    deque<pair<int,int>> Tiles;

    MagicTilesModel() {
        readFile();
        resetStartTime();
        initializeTiles();
    }

    int getAllTiles() {
        return allTiles;
    }
    int getNumOfTiles() {
        return numOfTiles;
    }
    int getNumOfDelTiles() {
        if (numOfDelTiles == 0)
            numOfDelTiles = 1;
        return numOfDelTiles;
    }
    int getStrick() {
        return strick;
    }
    int getScore() {
        return score;
    }

    void readFile(){
        ifstream file("map1.txt");  // Otwórz plik do odczytu
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

    void resetStartTime() {
        startTime = std::chrono::steady_clock::now(); // Resetowanie czasu
    }

    void updateGameLogic() {
        for (int i = 0; i < NUM_TRACKS; ++i) {
            for (int j = 0; j < MAX_TILES_PER_LINE; j++){
                if (tiles[i][j].isActive) {
                    tiles[i][j].y++;
                    if (tiles[i][j].y + TILE_HEIGHT - 1 >= HEIGHT + TILE_HEIGHT) {
                    // Kafelek dotarďż˝ na dďż˝ i nie zostaďż˝ strďż˝cony
                        lives--;  // Utrata punktu
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

    void handleTileHit(int track) {
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

    void initializeTiles() {
        for (int i = 0; i < NUM_TRACKS; ++i) {
            for (int j = 0; j < MAX_TILES_PER_LINE; j++) {
                tiles[i][j] = {0, START_Y, i, false};
            }
        }
    }

private:
    void spawnTile(int tracks) {
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

    void deleteTile(const Tile &tile) {
        for (int i = -1; i < TILE_HEIGHT; ++i) {
            for (int j = 0; j < TILE_WIDTH; ++j) {
                mvaddch(tile.y + i, tile.x + j, ' '); // Czyszczenie obszaru kafelka
            }
        }
        if (numOfDelTiles < allTiles) numOfDelTiles++;
        if (numOfDelTiles == allTiles) gameOver = true;
    }

    void updateScore() {
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
};

class ResultModel {
public:
    int visible_choices_result = 3; // Number of visible menu options
    int n_choices = sizeof(choices_result) / sizeof(char *);
};

class ResultView {
    WINDOW *result_win;  

public:
    ResultView() {
        // initscr();
        // start_color();
        // clear();
        // noecho();
        // curs_set(0);
        // cbreak();
        result_win = newwin(HEIGHT_M, WIDTH_M, 0, 0);
        keypad(result_win, TRUE);
        refresh();
    }

    ~ResultView() {
        delwin(result_win);
       // endwin();
    }

    void print_title(WINDOW *result_win, int tx, int ty) {
        int i, j;
        const char *line;
    
        for (i = 0; i < 5; i++) {  // 5 linii tytułu
            line = result[i];
            for (j = 0; line[j] != '\0'; j++) {
                if (line[j] != ' ') {  // Sprawdzanie, czy znak nie jest spacją
                    wattron(result_win, A_REVERSE | A_INVIS);  // Podświetlanie
                    mvwaddch(result_win, ty + i, tx + j, line[j]);
                    wattroff(result_win, A_REVERSE | A_INVIS);  // Wyłączanie podświetlenia
                } else {
                    mvwaddch(result_win, ty + i, tx + j, line[j]);  // Zwykły wydruk dla spacji
                }
            }
        }
        //mvwprintw(result_win, ty+5, tx, "%-64s", result[5]);
        wrefresh(result_win);  // Odświeżenie okna, aby zmiany były widoczne
    }

    void print_menu(int highlight, ResultModel &resultModel, MagicTilesModel &gameModel) {
        int x = WIDTH_M / 2 - 10;
        int y = HEIGHT_M / 2 - resultModel.visible_choices_result / 2 + 2;
        int tx = WIDTH_M / 2 - 20;
        int ty = HEIGHT_M / 2 - 10;

        box(result_win, 0, 0);
        print_title(result_win, tx, ty);
        mvwprintw(result_win, ty+6, tx - 6, "Score: %d", gameModel.score);
        mvwprintw(result_win, ty+6, tx + 11, "Acurccy: %.2f%% ",(float)gameModel.tilesTaken/gameModel.getNumOfDelTiles() * 100);
        mvwprintw(result_win, ty+6, tx + 33, "Highest series: %d", gameModel.maxStrick);

        for (int i = 0; i <resultModel.visible_choices_result; ++i) {
            int index = i * 3 + 1; // Main menu options are indexed by this
            if (highlight == i + 1) {  // Podświetlenie
                wattron(result_win, A_REVERSE | A_INVIS);
                mvwprintw(result_win, y-1, x, "%-10s", choices_result[index-1]);
                wattroff(result_win, A_INVIS);
                mvwprintw(result_win, y, x, "%-10s", choices_result[index]);  // Cały "przycisk"
                wattron(result_win, A_INVIS);
                mvwprintw(result_win, y+1, x, "%-10s", choices_result[index+1]);
                wattroff(result_win, A_REVERSE | A_INVIS);
            } else {
                wattron(result_win, A_INVIS);
                mvwprintw(result_win, y-1, x, "%-10s", choices_result[index-1]);
                wattroff(result_win, A_INVIS);
                mvwprintw(result_win, y, x, "%-10s", choices_result[index]);  // Niepodświetlony
                wattron(result_win, A_INVIS);
                mvwprintw(result_win, y+1, x, "%-10s", choices_result[index+1]);
                wattroff(result_win, A_INVIS);
            }
            y += 3; // Move to next option row
        }
        wrefresh(result_win);
    }

    WINDOW *get_window() { return result_win; }
};

class ResultController {
    ResultView &resultView;
    ResultModel &resultModel;
    int highlight;
    int choice;
    MagicTilesModel &gameModel;
    // MagicTilesView gameView;
    // MagicTilesController gameController;

public:
    ResultController(ResultView &resultView, ResultModel &resultModel, MagicTilesModel &gameModel) : resultView(resultView), resultModel(resultModel), highlight(1), choice(0), 
    gameModel(gameModel)/*, gameView(gameModel), gameController(gameModel, gameView)*/ {}

    int select_option() {
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
            if (choice == 1)  // Wybrano opcję
                break;
            else if (choice == 2) {
                choice = 0;
                break;
            }
            else if (choice == 3) {
                choice = 0;
                break;
            }
                
        }
        return choice;
    }
};

class MagicTilesView {
public:
    MagicTilesView(MagicTilesModel &m) : model(m) {
        init_pair(3, COLOR_WHITE, COLOR_BLUE);
        init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(1, COLOR_WHITE, COLOR_WHITE);
        init_pair(5, COLOR_WHITE, COLOR_BLACK);
        bkgd(COLOR_PAIR(5));
    }

    ~MagicTilesView() { 
        clear();
        //endwin(); 
        //printf("allTiles: %d, numOfTiles: %d, numOfDelTiles: %d\tSeria: %d Wynik: %d\n", model.getAllTiles(), model.getNumOfTiles(), model.getNumOfDelTiles(), model.getStrick(), model.getScore());
    }

    void drawGame(const MagicTilesModel &model) {
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

    void drawGameOver(const MagicTilesModel &model) {
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

private:
    MagicTilesModel &model;

    void drawTile(const Tile &tile) {
        if (tile.y > START_Y) {
            for (int j = 0; j < TILE_WIDTH; ++j) {
                mvaddch(tile.y - 1, tile.x + j, ' ');  // Czyszczenie poprzedniej gďż˝rnej linii kafelka

            }
        }

        // Rysowanie dolnej linii kafelka
        for (int j = 0; j < TILE_WIDTH; ++j) {
            attron(COLOR_PAIR(1));
            mvaddch(tile.y + TILE_HEIGHT - 1, tile.x + j, TILE_CHAR);  // Rysowanie tylko dolnej linii kafelka
            attroff(COLOR_PAIR(1));
        }
    }

    void drawUI(const MagicTilesModel &model) {
        mvprintw(1, 1, "Score: %d", model.score);
        mvprintw(2, 1, "Lives: %d", model.lives);
        mvprintw(3, 1, "Series: %d   ", model.strick);
        mvprintw(1, 34, "Controls: h - track 1, j - track 2, k - track 3, l - track 4");
        mvprintw(2, 52, "Press q to exit");

        int beamY = HEIGHT - 1;
        attron(COLOR_PAIR(4));
        mvhline(beamY, 0, ' ', 120);
        mvhline(29, 0, ' ', 119);
        mvhline(0, 0, ' ', 119);
        mvhline(4, 0, ' ', 119);
        mvvline(0 , 119, ' ', 30);
        mvvline(0 , 0, ' ', 30);
        attroff(COLOR_PAIR(4));
    }
};

class MagicTilesController {
public:
    MagicTilesController(MagicTilesModel &gameModel, MagicTilesView &gameView)
        : gameModel(gameModel), gameView(gameView) {}

    int run() {
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
        ResultView resultView;
        ResultController resultController(resultView, resultModel, gameModel);
        resultView.print_menu(1, resultModel, gameModel);
        choice = resultController.select_option();
        return choice;
    }

private:
    MagicTilesModel &gameModel;
    MagicTilesView &gameView;

    void processInput() {
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
};

class MenuModel {
public:
    int visible_choices = 3; // Number of visible menu options
    int n_choices = sizeof(choices_menu) / sizeof(char *);
};

class MenuView {
    WINDOW *menu_win;

public:
    MenuView() {
        initscr();
        start_color();
        clear();
        noecho();
        curs_set(0);
        cbreak();
        menu_win = newwin(HEIGHT_M, WIDTH_M, 0, 0);
        keypad(menu_win, TRUE);
        refresh();
    }

    ~MenuView() {
        delwin(menu_win);
        endwin();
    }

    void print_title(WINDOW *menu_win, int tx, int ty) {
        int i, j;
        const char *line;
    
        for (i = 0; i < 5; i++) {  // 5 linii tytułu
            line = title[i];
            for (j = 0; line[j] != '\0'; j++) {
                if (line[j] != ' ') {  // Sprawdzanie, czy znak nie jest spacją
                    wattron(menu_win, A_REVERSE | A_INVIS);  // Podświetlanie
                    mvwaddch(menu_win, ty + i, tx + j, line[j]);
                    wattroff(menu_win, A_REVERSE | A_INVIS);  // Wyłączanie podświetlenia
                } else {
                    mvwaddch(menu_win, ty + i, tx + j, line[j]);  // Zwykły wydruk dla spacji
                }
            }
        }
        mvwprintw(menu_win, ty+5, tx, "%-64s", title[5]);
        wrefresh(menu_win);  // Odświeżenie okna, aby zmiany były widoczne
    }

    void print_menu(int highlight, MenuModel &model) {
        int x = WIDTH_M / 2 - 10;
        int y = HEIGHT_M / 2 - model.visible_choices / 2 + 2;
        int tx = WIDTH_M / 2 - 30;
        int ty = HEIGHT_M / 2 - 10;

        box(menu_win, 0, 0);
        print_title(menu_win, tx, ty);

        for (int i = 0; i < model.visible_choices; ++i) {
            int index = i * 3 + 1; // Main menu options are indexed by this
            if (highlight == i + 1) {  // Podświetlenie
                wattron(menu_win, A_REVERSE | A_INVIS);
                mvwprintw(menu_win, y-1, x, "%-10s", choices_menu[index-1]);
                wattroff(menu_win, A_INVIS);
                mvwprintw(menu_win, y, x, "%-10s", choices_menu[index]);  // Cały "przycisk"
                wattron(menu_win, A_INVIS);
                mvwprintw(menu_win, y+1, x, "%-10s", choices_menu[index+1]);
                wattroff(menu_win, A_REVERSE | A_INVIS);
            } else {
                wattron(menu_win, A_INVIS);
                mvwprintw(menu_win, y-1, x, "%-10s", choices_menu[index-1]);
                wattroff(menu_win, A_INVIS);
                mvwprintw(menu_win, y, x, "%-10s", choices_menu[index]);  // Niepodświetlony
                wattron(menu_win, A_INVIS);
                mvwprintw(menu_win, y+1, x, "%-10s", choices_menu[index+1]);
                wattroff(menu_win, A_INVIS);
            }
            y += 3; // Move to next option row
        }
        wrefresh(menu_win);
    }

    void exitScreen () {
        clear();
        mvprintw(HEIGHT_M / 2, WIDTH_M / 2 - 8, "Thank you for your attention!");
    }

    WINDOW *get_window() { return menu_win; }
};

class MenuController {
    MenuView &view;
    MenuModel &model;
    int highlight;
    int choice;
    int c;
    // MagicTilesModel gameModel;
    // MagicTilesView gameView;
    // MagicTilesController gameController;

public:
    MenuController(MenuView &view, MenuModel &model) : view(view), model(model), highlight(1), choice(0)/*, 
    gameModel(), gameView(gameModel), gameController(gameModel, gameView)*/ {}

    void select_option() {
        WINDOW *menu_win = view.get_window();
        while (true) {
            if (choice != 1)
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
            view.print_menu(highlight, model);
            if (choice == 1) {
                MagicTilesModel gameModel;
                MagicTilesView gameView(gameModel);
                MagicTilesController gameController(gameModel, gameView);

                choice = gameController.run();  // Uruchomienie gry
                view.print_menu(highlight, model);
            } 
            else if (choice == 2) {
                // Obsługa opcji 2
                choice = 0;  // Resetowanie wyboru
            } 
            else if (choice == 3) {
                // Wyjście z menu
                view.exitScreen();
                getch();
                break;  // Wyjście z pętli
            }
        }
    }
};



int main() {
    MenuModel menuModel;
    MenuView menuView;
    MenuController menuController(menuView, menuModel);

    menuView.print_menu(1, menuModel);

    menuController.select_option();

    clear();

    return 0;
}
