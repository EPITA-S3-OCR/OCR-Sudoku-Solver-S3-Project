#include <stdio.h>
#include "solver.h"
#include "err.h"

int main(int argc, char *argv[]){
  // declare and initialize an array

  if(argc != 2)
          errx(1,"bad");

  const size_t SI = 9;
    
 

  char sudoku[SI][SI];
  loadSudoku(sudoku, argv[1], 9);


  // display 2D array using for loop
  printf("The Array elements are:\n");

  // outer loop for row
  for(int i=0; i<9; i++) {
    // inner loop for column
    for(int j=0; j<9; j++) {
      printf("%c", sudoku[i][j]);
    }
    printf("\n"); // new line
  }

  Solve(sudoku);

  printf("----------------\n");
  // outer loop for row
  for(int i=0; i<9; i++) {
    // inner loop for column
    for(int j=0; j<9; j++) {
      printf("%c", sudoku[i][j]);
    }
    printf("\n"); // new line
  }


  return 0;
}
