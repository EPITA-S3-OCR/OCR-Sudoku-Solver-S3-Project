#include "main.h"

int solverMain(int argc, char *argv[])
{
  // declare and initialize an array

  if (argc != 2)
    errx(1, "bad");

  const size_t SI = 9;

  char sudoku[SI][SI];

  loadSudoku(sudoku, argv[1]);

  if (Solve(sudoku) == -1)
  {
    errx(1, "Could not be solved :(");
  }

  else
  {
    char  actual_path[2048];
    char *full_path = realpath(argv[1], actual_path);

    // Grab filename.

    char *ptr_to_filename = strrchr(full_path, '/');

    // Get path until /solver.
    char *ptr_to_rest = strstr(full_path, "/tests/solver");

    // Index until /solver
    int pos = ptr_to_rest - full_path;
    char path_for_newfile[2048];
    strncpy(path_for_newfile, full_path, pos);

    // Now we put everything together!
    path_for_newfile[pos] = '\0';

    strcat(path_for_newfile, "/output/results");
    strcat(path_for_newfile, ptr_to_filename);
    strcat(path_for_newfile, ".result");

    createFile(sudoku, path_for_newfile);
  }

  return 0;
}
