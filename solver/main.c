#include <stdio.h>
#include "stdlib.h"
#include "libgen.h"
#include "string.h"
#include "solver.h"
#include "err.h"

int main(int argc, char *argv[]){
    // declare and initialize an array

    if(argc != 2)
        errx(1,"bad");

    const size_t SI = 9;

    char sudoku[SI][SI];

    loadSudoku(sudoku, argv[1], 9);

    if (Solve(sudoku) == -1){
        errx(1,"Could not be solved :(");
    }

    else{

        char trailing[] = ".result";
        char where_to_save[] = "results";

        char *path = strdup(argv[1]);
        char* whoami;
        (whoami = strrchr(path, '/')) ? ++path : (whoami = path);




        strcat(where_to_save, whoami);
        strcat(where_to_save, trailing);

        createFile(sudoku,where_to_save);

    }

    return 0;
}
