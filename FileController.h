#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

#include "FileModel.h"
#include "FileView.h"

class FileController {
    FileView &fileView;
    FileModel &fileModel;
    int highlight;
    int choice;

public:
    // Constructor to initialize the controller with views, models, and game state
    FileController(FileView &fileView, FileModel &fileModel);

    // Function to handle option selection in the result menu
    int file_select_option();
};

#endif // FILE_CONTROLLER_H
