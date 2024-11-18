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
#include "FileModel.h"
#include "FileView.h"
#include "FileController.h"
#include "MagicTilesView.h"
#include "MagicTilesController.h"
#include "MenuModel.h"
#include "MenuView.h"
#include "MenuController.h"

using namespace std;

int main() {
    MenuModel menuModel;
    MenuView menuView;
    MenuController menuController(menuView, menuModel);

    menuView.print_menu(1, menuModel);

    menuController.select_option();

    clear();

    return 0;
}