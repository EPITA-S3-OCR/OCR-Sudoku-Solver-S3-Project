#include <dirent.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef HELPERS_H
#  define HELPERS_H

/// @brief Returns the max index of an array
/// @param array The given array
/// @param len The length of the given array
/// @return The max index of the given array
size_t    arrayMaxIndex(double *array, size_t len);

/// @brief Frees the given 2D array
/// @param array The given 2D array
/// @param len The length of the given 2D array
void      free2dArray(double **array, size_t len);

/// @brief Fill the given 2D array with random values
/// @param array The given 2D array
/// @param len The length of the given 2D array
/// @param max The max value of the random values
void      arrayFillUniform(double *array, size_t len, double max);

/// @brief Shuffles every element of the given array
/// @param array The given array
/// @param len The length of the given array
void      arrayShuffle(size_t *array, size_t len);

/// @brief Print the given array
/// @param array The given array
/// @param len The length of the given array
void      print1dArray(double *array, size_t len);

/// @brief Print the given 2D array
/// @param array The given 2D array
/// @param len1 The length of the first dimension of the given 2D array
/// @param len2 The length of the second dimension of the given 2D array
void      print2dArray(double **array, size_t len1, size_t len2);

/// @brief Allocates & initializes a 3D array
/// @param s1 The length of the first dimension of the 3D array
/// @param s2 The length of the second dimension of the 3D array
/// @param s3 The length of the third dimension of the 3D array
/// @return The allocated & initialized 3D array
double  **init2dArray(size_t s1, size_t s2);
double ***init3dArray(size_t s1, size_t s2, size_t s3);

/// @brief Count the number of folders in a given directory
/// @param folderRoot The given path to the directory
/// @return The number of folders in the given directory
size_t countFolders(char *folderRoot);

/// @brief Allocates & initializes a identity matrix
/// @param size The size of the identity matrix
/// @return The allocated & initialized identity matrix
double  **createIdentityMatrix(size_t size);
#endif