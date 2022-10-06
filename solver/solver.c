#include "stdio.h"
#include "string.h"
#include "stdlib.h"

const size_t SIZE = 9;


int possiblePlacement(int row, int col, char num, char grid[SIZE][SIZE]){

    //Lets check if there is a possible placement on a row
    for (size_t i = 0; i < SIZE; i++) {
        if(grid[row][i] == num)
            return -1;
    }

    //Lets check if there is a possible placement on a col

    for (size_t i = 0; i < SIZE; i++) {
        if(grid[i][col] == num)
            return -1;
    }

    //Little trick to find which subsquare we are on.

    int subrow = (row/3)*3;
    int subcol = (col/3)*3;

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            if(grid[subrow + i][subcol + j] == num)
                return -1;
        } 
    }

    //This means the position placement is valid!

    return 1;
}


int Solve(char grid[SIZE][SIZE]){

    //Find empty position to place a number

    //By default 'a', if its still 'a' after loop then
    //there must be no empty positions.

    char row_current = 'a';
    char coll_current;

    for (size_t i = 0; i < SIZE; i++) {
        for (size_t j = 0; j < SIZE; j++) {
            if(grid[i][j] == '0'){
                row_current = i;
                coll_current = j;
            }
        } 
    }

    if(row_current == 'a') // Sudoku is full
        return 1;

    for (size_t i = 1; i < SIZE+1; i++) {

        // If you cant place the value continue

        if(possiblePlacement(row_current, coll_current, 
                    grid[row_current][coll_current],grid) == -1)
            continue;

        //Now we try the possbile value

        grid[row_current][coll_current] = '0' + i;

        if(Solve(grid))
            return 1;

        //Getting here means the value sucked, so we try another one.

        grid[row_current][coll_current] = '0';
    }

    return -1;
}
