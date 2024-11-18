#ifndef FILE_MODEL_H
#define FILE_MODEL_H



class FileModel {
public:
    int visible_choices_result_y;  // Number of visible menu options in the result menu
    //int visible_choices_result_x; 
    int n_choices;               // Total number of available options
    FileModel();

    // Constructor to initialize visible choices and total choices
    //ResultModel() : visible_choices_result(3), n_choices(sizeof(choices_result) / sizeof(char *)) {}
};

#endif // FILE_MODEL_H
