#ifndef HELPERS_H
#define HELPERS_H

void free2dArray(double **array, size_t len);
void arrayFillUniform(double *array, size_t len, double max);
void arrayShuffle(size_t *array, size_t len);
void print1dArray(double *array, size_t len);
void print2dArray(double **array, size_t len1, size_t len2);

#endif