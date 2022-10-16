#include <stdio.h>
#include "stdlib.h"
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

        // If we dont get the full path it could lead to problems
        // This lets us call the executable from anywhere.


        char actual_path[2048];
        char* full_path = realpath(argv[1], actual_path);

        // Grab filename.

        char* ptr_to_filename = strrchr(full_path, '/');

        // Get path until /solver.

        char* ptr_to_rest = strstr(full_path, "/examples");


        // Index until /solver

        int pos = ptr_to_rest - full_path;

        char path_for_newfile[2048];
        strncpy(path_for_newfile, full_path, pos);


        // Now we put everything together!

        strcat(path_for_newfile, "/results");
        strcat(path_for_newfile, ptr_to_filename);
        strcat(path_for_newfile, ".result");

        createFile(sudoku, path_for_newfile);

    }

    return 0;

}


