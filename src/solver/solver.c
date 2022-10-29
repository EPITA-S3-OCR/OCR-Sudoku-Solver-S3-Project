#include "solver.h"

int const S = 9;

int possiblePlacement(int row, int col, char num, char grid[][9])
{

  // Lets check if there is a possible placement on a row
  //
  // Lets check if there is a possible placement on a col

  for (size_t i = 0; i < S; i++)
  {
    if (grid[row][i] == num)
      return -1;

    if (grid[i][col] == num)
      return -1;
  }

  // Little trick to find which subsquare we are on.

  int subrow = (row / 3) * 3;
  int subcol = (col / 3) * 3;

  for (size_t i = 0; i < 3; i++)
  {
    for (size_t j = 0; j < 3; j++)
    {
      if (grid[subrow + i][subcol + j] == num)
        return -1;
    }
  }

  // This means the position placement is valid!

  return 1;
}

int findEmpty(char grid[][9], int *row, int *column)
{
  for (int x = 0; x < 9; x++)
  {
    for (int y = 0; y < 9; y++)
    {
      if (grid[x][y] == '0')
      {
        *row    = x;
        *column = y;

        return 1;
      }
    }
  }
  return -1;
}

int Solve(char grid[][9])
{

  // Find empty position to place a number

  int row_current;
  int coll_current;

  if (findEmpty(grid, &row_current, &coll_current) == -1)
    return 1;

  for (size_t i = 1; i <= S; i++)
  {

    // If you cant place the value continue

    if (possiblePlacement(row_current, coll_current, '0' + i, grid) == 1)
    {

      grid[row_current][coll_current] = '0' + i;

      if (Solve(grid) == 1)
        return 1;

      grid[row_current][coll_current] = '0';
    }
  }

  return -1;
}

int createFile(char grid[][9], char *path)
{

  // Looks like 123 456 789\n ...

  /* File pointer to hold reference to our file */
  FILE *fPtr;

  /* Where to create it  */
  fPtr = fopen(path, "w");

  /* fopen() return NULL if last operation was unsuccessful */

  if (fPtr == NULL)
  {
    /* File not created hence exit */
    errx(EXIT_FAILURE, "Unable to open file");
  }

  for (size_t i = 0; i < S; i++)
  {
    if (i == 3 || i == 6)
      fputc('\n', fPtr);
    for (size_t j = 0; j < S; j++)
    {
      fputc(grid[i][j], fPtr);
      if (j == 2 || j == 5)
        fputc(' ', fPtr);
    }
    fputc('\n', fPtr);
  }

  /* Close file to save file data */
  fclose(fPtr);

  return 0;
}

char *loadSudoku(char sudoku[][9], char path[])
{

  // initialize a 2 dimensional matrix

  size_t i       = 0;
  size_t j       = 0;
  int    counter = 0;

  // the file and the character that will be read
  FILE *file;
  char  ch;

  // find how to open file with relative path
  // go through the file and add to the array

  if ((file = fopen(path, "r")) == NULL)
  {
    printf("Error opening file!");
    exit(1);
  }

  ch = fgetc(file); // Grab the first character

  while (ch != EOF)
  {

    // printf("%c -> i = %zu, j = %zu\n",ch,i,j);

    if (ch == '\n')
    {
      ch = fgetc(file); // Remove as much \n as you need.
      continue;
    }

    if (ch == '.')
    {
      sudoku[i][j] = '0';
      j += 1;
    }

    else if (ch != ' ' && ch != EOF)
    {
      sudoku[i][j] = ch;
      j += 1;
    }

    counter += 1;

    if (counter == 11)
    {
      counter = 0;
      // printf("\n%i------\n",i);
      i += 1;
      j = 0;
    }

    ch = fgetc(file);
  }

  // close file
  fclose(file);

  return basename(path);
}
