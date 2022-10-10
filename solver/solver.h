#pragma once
// Reads sudoku from file
#include "stddef.h"

const int SIZE = 9;

int Solve(char grid[SIZE][SIZE]);
int createFile(char grid[SIZE][SIZE], char* path);
int possiblePlacement(int row, int col, char num, char grid[SIZE][SIZE]);
char** loadFile(char path[], size_t s);
