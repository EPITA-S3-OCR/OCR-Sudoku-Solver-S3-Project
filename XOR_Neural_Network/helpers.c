#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

void free2dArray(double **array, size_t len)
{
  // Free every subarrays of the given 2D-array
  for (size_t i = 0; i < len; i++)
    free(array[i]);
}

void arrayFillUniform(double *array, size_t len, double max)
{
  // Fill the given array with random values x such that 0 <= x <= max
  for (size_t i = 0; i < len; i++)
    array[i] = ((double)rand() / RAND_MAX) * max;
}

void arrayShuffle(size_t *array, size_t len)
{
  // Prevent unexpected behaviour
  if (len > 1)
  {
    // Shuffle the given array
    for (size_t i = 0; i < len - 1; i++)
    {
      // Generate a random index
      size_t j = i + rand() / (RAND_MAX / (len - i) + 1);

      // Swap both values
      int temp = array[j];
      array[j] = array[i];
      array[i] = temp;
    }
  }
}

void print1dArray(double *array, size_t len)
{
  printf("[");
  for (size_t i = 0; i < len; i++)
  {
    if (i < len - 1)
      printf("%lf, ", array[i]);
    else
      printf("%lf", array[i]);
  }
  printf("]\n");
}

void print2dArray(double **array, size_t len1, size_t len2)
{
  printf("[");
  for (size_t i = 0; i < len1; i++)
  {
    printf("[");
    for (size_t j = 0; j < len2; j++)
    {
      if (j < len2 - 1)
        printf("%lf, ", array[i][j]);
      else
        printf("%lf", array[i][j]);
    }

    if (i < len1 - 1)
      printf("], ");
    else
      printf("]");
  }
  printf("]\n");
}