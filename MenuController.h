#ifndef MENU_CONTROLLER_H
#define MENU_CONTROLLER_H

#include "MagicTilesModel.h"
#include "MagicTilesView.h"
#include "MagicTilesController.h"
#include "FileModel.h"
#include "FileView.h"
#include "FileController.h"
#include "MenuModel.h"
#include "MenuView.h"


class MenuController {
public:
    // Constructor: Initializes the controller with references to the view and model
    MenuController(MenuView &view, MenuModel &model);

    // Starts the menu and handles the option selection
    void select_option();

private:
    MenuView &view;      // Reference to the menu view
    MenuModel &model;    // Reference to the menu model
    int highlight;       // Currently highlighted option in the menu
    int choice;          // Option selected by the user
    int c;               // Key input from the user
    int file;
};

#endif // MENU_CONTROLLER_H
