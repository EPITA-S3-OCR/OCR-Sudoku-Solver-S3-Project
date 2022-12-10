#include <dirent.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef HELPERS_H
#  define HELPERS_H

size_t    arrayMaxIndex(double *array, size_t len);
void      free2dArray(double **array, size_t len);
void      arrayFillUniform(double *array, size_t len, double max);
void      arrayShuffle(size_t *array, size_t len);
void      print1dArray(double *array, size_t len);
void      print2dArray(double **array, size_t len1, size_t len2);
double    getDouble(char *str);
int       getBinaryInt(int n);
int       getDecimalInt(int n);
char     *getBinaryString(int n);
double  **init2dArray(size_t s1, size_t s2);
double ***init3dArray(size_t s1, size_t s2, size_t s3);
size_t    countFolders(char *folderRoot);
double  **createIdentityMatrix(size_t size);
#endif