#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif
#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver.h"

/**
 * @brief The main for the solver. Calls the solver.
 * @param argc The number of arguments
 * @param argv The array of arguments given by the user.
 * @return Returns exit code
 */
int solverMain(int argc, char *argv[]);

/**
 * @brief The main for the solver used by the UI
 * @param sudokuPath Path to the sudoku
 * @param length The length of the sudoku <9 or 16>
 * @return Returns exit code
 */
int solverMainUi(char *sudokuPath, int length);
