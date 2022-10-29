#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libgen.h"

// Reads sudoku from file

// File handling stuff
char *loadSudoku(char sudoku[][9], char path[]);
int   createFile(char grid[][9], char *path);

// Solving sudoku algortihms.
int Solve(char grid[][9]);
int possiblePlacement(int row, int col, char num, char grid[][9]);
