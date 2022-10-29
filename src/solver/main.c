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

    // If we dont get the full path it could lead to problems
    // This lets us call the executable from anywhere.

    char  actual_path[2048];
    char *full_path = realpath(argv[1], actual_path);

    // Grab filename.

    char *ptr_to_filename = strrchr(full_path, '/');

    char *basepath  = "./output/results";
    char *extension = ".result";
    char *path      = malloc(strlen(basepath) + strlen(ptr_to_filename)
                             + strlen(extension) + 1);
    strcat(path, basepath);
    strcat(path, ptr_to_filename);
    strcat(path, ".result");
    createFile(sudoku, path);
  }

  return 0;
}
