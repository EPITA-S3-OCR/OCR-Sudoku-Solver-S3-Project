#include "solver.h"

int possiblePlacement(int row, int col, char num, char grid[][16], size_t length)
{

  // Lets check if there is a possible placement on a row
  //
  // Lets check if there is a possible placement on a col

  for (size_t i = 0; i < length; i++)
  {
    if (grid[row][i] == num)
      return -1;

    if (grid[i][col] == num)
      return -1;
  }

  // Little trick to find which subsquare we are on.

  int subrow; 
  int subcol; 
  size_t boxSize;

  if (length == 9)
  {
      subrow = (row / 3) * 3;
      subcol = (col / 3) * 3;
      boxSize = 3;
  }

  else 
  {
      subrow = (row / 4) * 4;
      subcol = (col / 4) * 4;
      boxSize = 4;
  }

  for (size_t i = 0; i < boxSize; i++)
  {
    for (size_t j = 0; j < boxSize; j++)
    {
      if (grid[subrow + i][subcol + j] == num)
        return -1;
    }
  }

  // This means the position placement is valid!

  return 1;
}

int findEmpty(char grid[][16], int *row, int *column, size_t length)
{
  for (size_t x = 0; x < length; x++)
  {
    for (size_t y = 0; y < length; y++)
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

int Solve(char grid[][16], size_t length)
{

  // Find empty position to place a number

  int row_current;
  int coll_current;

  if (findEmpty(grid, &row_current, &coll_current, length) == -1)
    return 1;

  for (size_t i = 1; i <= length; i++)
  {

    // If you cant place the value continue
    
    int offset = i > 9 ? 7 : 0;
    if (possiblePlacement(row_current, coll_current, '0' + i + offset, grid, length) == 1)
    {

      grid[row_current][coll_current] = '0' + i + offset;

      if (Solve(grid, length) == 1)
        return 1;

      grid[row_current][coll_current] = '0';
    }
  }

  return -1;
}

int createFile(char grid[][16], char *path, size_t length)
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

  if (length == 9)
  {
      for (size_t i = 0; i < length; i++)
      {
          if (i == 3 || i == 6)
              fputc('\n', fPtr);
          for (size_t j = 0; j < length; j++)
          {
              fputc(grid[i][j], fPtr);
              if (j == 2 || j == 5)
                  fputc(' ', fPtr);
          }
          fputc('\n', fPtr);
      }
  }
  
  else 
  {
      for (size_t i = 0; i < length; i++)
      {
          if (i == 4 || i == 8 || i == 12)
              fputc('\n', fPtr);
          for (size_t j = 0; j < length; j++)
          {
              fputc(grid[i][j], fPtr);
              if (j == 3 || j == 7 || j == 11)
                  fputc(' ', fPtr);
          }
          fputc('\n', fPtr);
      }
  }

  /* Close file to save file data */

  fclose(fPtr);

  return 0;
}

char *loadSudoku(char sudoku[][16], char path[], size_t length)
{

  // initialize a 2 dimensional matrix

  size_t i       = 0;
  size_t j       = 0;
  int    counter = 0;
  int maxCounter = length == 9 ? 11 : 19;

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

    if (counter == maxCounter)
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
