#ifndef RESULT_MODEL_H
#define RESULT_MODEL_H



class ResultModel {
public:
    int visible_choices_result;  // Number of visible menu options in the result menu
    int n_choices;               // Total number of available options
    ResultModel();

    // Constructor to initialize visible choices and total choices
    //ResultModel() : visible_choices_result(3), n_choices(sizeof(choices_result) / sizeof(char *)) {}
};

#endif // RESULT_MODEL_H
