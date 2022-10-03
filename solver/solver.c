#include "stdio.h"
#include "stdlib.h"

char** loadFile(char path[], size_t s) {
    FILE* file;
    char ch;
    size_t row = 0;
    size_t col = 0;

    char sudoku[s][s];

    file = fopen(path, "r");

    if (file == NULL) {
        printf("file cannot be opened");
    }

    

    fclose(file);
    return &sudoku;
}
 
