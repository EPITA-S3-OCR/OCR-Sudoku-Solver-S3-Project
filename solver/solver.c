#include "stdio.h"
#include "solver.h"
#include "err.h"
#include "string.h"
#include "stdlib.h"

int const S = 9;

int possiblePlacement(int row, int col, char num, char grid[S][S]){

    //Lets check if there is a possible placement on a row
    for (size_t i = 0; i < S; i++) {
        if(grid[row][i] == num)
            return -1;
    }

    //Lets check if there is a possible placement on a col

    for (size_t i = 0; i < S; i++) {
        if(grid[i][col] == num)
            return -1;
    }

    //Little trick to find which subsquare we are on.

    int subrow = row - row % 3;
    int subcol = col - col % 3;

    printf("%i and %i",subrow,subcol);

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            if(grid[subrow + i][subcol + j] == num)
                return -1;
        } 
    }

    //This means the position placement is valid!

    return 1;
}


int Solve(char grid[S][S]){

    //Find empty position to place a number

    //By default 'a', if its still 'a' after loop then
    //there must be no empty positions.

    char row_current = 'a';
    char coll_current;

    for (size_t i = 0; i < S; i++) {
        for (size_t j = 0; j < S; j++) {
            if(grid[i][j] == '0'){
                row_current = i;
                coll_current = j;
            }
        } 
    }
 
    if(row_current == 'a') // Sudoku is full
        return 1;

    for (size_t i = 1; i < S+1; i++) {

        // If you cant place the value continue

        if(possiblePlacement(row_current, coll_current, 
                    grid[row_current][coll_current],grid) == 1){


            grid[row_current][coll_current] = '0' + i;
            if(Solve(grid) == 1)
                return 1;
        }

        //Getting here means the value sucked, so we try another one.


        //Now we try the possbile value

        grid[row_current][coll_current] = '0';
    }

    return -1;
}

int createFile(char grid[S][S], char* path){

    size_t DATA_S = 200; // Dimensions look like 12 * 11, but Just
                            // to be sure.


    char data[DATA_S];
    // Looks like 123 456 789\n ...

    /* File pointer to hold reference to our file */
    FILE * fPtr;


    /* Just where to create it  */

    fPtr = fopen(path, "w");


    /* fopen() return NULL if last operation was unsuccessful */
    if(fPtr == NULL)
    {
        /* File not created hence exit */
        errx(EXIT_FAILURE, "Unable to open file");   
    }

    size_t data_curr_iter = 0;

    for (size_t i = 0; i <S ; i++) {     
        if(i%3 == 0){
            data[data_curr_iter] = '\n';
            data_curr_iter += 1;
            continue; // Helps us avoid some iterations of j.
        }
        for (size_t j = 0; j <S ; j++) {
            if(j%3 == 0)
                data[data_curr_iter] = ' ';
            else {
                data[data_curr_iter] = grid[i][j];  
            }

            data_curr_iter += 1;

        }
    }

    // Add extra two bottom lines.

    for (size_t i = 0; i < 2; i++) {
        data[data_curr_iter] = '\n';
        data_curr_iter += 1;
    }

    //We can symbolize the end of sentence by placing a one.

    data[data_curr_iter] = 0;

    /* Write data to file */
    fputs(data, fPtr);


    /* Close file to save file data */
    fclose(fPtr);


    /* Success message */
    printf("File created and saved successfully. :) \n");


    return 0;
}
        
int loadSudoku(char sudoku[S][S],char path[], size_t s) {

    // initialize a 2 dimensional matrix

    size_t i = 0;
    size_t j = 0;
    int counter = 0;

    // the file and the character that will be read
    FILE* file;
    char ch;

    // find how to open file with relative path
    // go through the file and add to the array

    if ((file = fopen(path, "r")) == NULL) {
        printf("Error opening file!");
        exit(1);
    }

    ch = fgetc(file); //Grab the first character

    while (ch != EOF) {

        //printf("%c -> i = %zu, j = %zu\n",ch,i,j);

        if(ch == '\n'){
            ch = fgetc(file); // Remove as much \n as you need.
            continue;
        }

        if (ch == '.') {
            sudoku[i][j] = '0';
            j += 1;
        }

        else if (ch != ' ' && ch != EOF){
            sudoku[i][j] = ch;
            j += 1;
        }

        counter += 1;

        if (counter == 11) {
            counter = 0;
            //printf("\n%i------\n",i);
            i += 1;
            j = 0;
        }

        ch = fgetc(file);
    }
    
    

    // close file
    fclose(file);

    return 0;
}


