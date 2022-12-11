#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif
#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver.h"

int solverMain(int argc, char *argv[]);
int solverMainUi(char *sudokuPath, int length);