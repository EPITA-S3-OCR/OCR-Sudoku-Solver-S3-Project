#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libgen.h"

// Reads sudoku from file

// File handling stuff

/**
 * @brief Loads a txt file into the sudoku. 
 * @param sudoku A 16x16 sudoku array of char to be filled
 * @param path Path to the file to be loaded
 * @param length The length of the Sudoku
 * @return A null-terminated string 
 */
char *loadSudoku(char sudoku[][16], char path[], size_t length);

/**
 * @brief Loads a sudoku into a file. 
 * @param sudoku A 16x16 sudoku array of char
 * @param path Path to the file to be loaded in
 * @param length The length of the Sudoku
 * @return 0 if successful, 1 else.
 */
int   createFile(char grid[][16], char *path, size_t length);

// Solving sudoku algortihms.

/**
 * @brief Solves a Sudoku. 
 * @param sudoku A 16x16 sudoku array of char to be solved
 * @param length The length of the Sudoku
 * @return 1 if successful, -1 else.
 */
int Solve(char grid[][16], size_t length);

/**
 * @brief Checks if num can be placed in the cell row col. 
 * @param row The row to check
 * @param col The col to check
 * @param grid The sudoku to fill
 * @param length of the sudoku
 * @return 1 if possible, -1 else.
 */
int possiblePlacement(int row, int col, char num, char grid[][16], size_t length);
