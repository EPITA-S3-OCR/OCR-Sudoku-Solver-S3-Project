#pragma once
// Reads sudoku from file
#include "stddef.h"


int Solve(char grid[9][9]);
int createFile(char grid[9][9], char* path);
char** loadSudoku(char path[], size_t s);
int possiblePlacement(int row, int col, char num, char grid[9][9]);
char** loadFile(char path[], size_t s);
