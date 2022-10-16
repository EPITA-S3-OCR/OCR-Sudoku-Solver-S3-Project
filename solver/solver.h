#pragma once
// Reads sudoku from file
#include "stddef.h"

// File handling stuff
char* loadSudoku(char sudoku[][9], char path[]);
int createFile(char grid[][9], char* path);

//Solving sudoku algortihms.
int Solve(char grid[][9]);
int possiblePlacement(int row, int col, char num, char grid[][9]);
