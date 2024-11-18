#ifndef RESULT_CONTROLLER_H
#define RESULT_CONTROLLER_H

#include "MagicTilesModel.h"
#include "ResultModel.h"
#include "ResultView.h"

class ResultController {
    ResultView &resultView;
    ResultModel &resultModel;
    int highlight;
    int choice;
    MagicTilesModel &gameModel;

public:
    // Constructor to initialize the controller with views, models, and game state
    ResultController(ResultView &resultView, ResultModel &resultModel, MagicTilesModel &gameModel);

    // Function to handle option selection in the result menu
    int select_option();
};

#endif // RESULT_CONTROLLER_H
