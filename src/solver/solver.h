#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libgen.h"

// Reads sudoku from file

// File handling stuff
char *loadSudoku(char sudoku[][16], char path[], size_t length);
int   createFile(char grid[][16], char *path, size_t length);

// Solving sudoku algortihms.
int Solve(char grid[][16], size_t length);
int possiblePlacement(int row, int col, char num, char grid[][16], size_t length);
