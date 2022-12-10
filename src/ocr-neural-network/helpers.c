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

size_t arrayMaxIndex(double *array, size_t len)
{
  // Find the index of the maximum value in the given array
  size_t maxIndex = 0;
  for (size_t i = 1; i < len; i++)
    if (array[i] > array[maxIndex])
      maxIndex = i;
  return maxIndex;
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

// get from binary int to int
int getDecimalInt(int n)
{
  int decimal = 0;
  int i       = 0;
  int remainder;
  while (n != 0)
  {
    remainder = n % 10;
    n /= 10;
    decimal += remainder * pow(2, i);
    ++i;
  }
  return decimal;
}

// get binary string from int
char *getBinaryString(int n)
{
  char *binary = malloc(9);
  for (int i = 7; i >= 0; i--)
  {
    binary[i] = n % 2 + '0';
    n /= 2;
  }
  binary[8] = '\0';
  return binary;
}

double **createIdentityMatrix(size_t size)
{
  // Initialize the training outputs
  double **array2d = malloc(size * sizeof(double *));
  if (array2d == NULL)
  {
    fprintf(stderr, "Error: malloc failed\n");
  }
  for (size_t i = 0; i < size; i++)
  {
    array2d[i] = malloc(size * sizeof(double));
    if (array2d[i] == NULL)
    {
      errx(1, "malloc failed");
    }
  }

  for (size_t i = 0; i < size; i++)
  {
    // Initialize the current training output
    for (size_t j = 0; j < size; j++)
    {
      // Everything should be 0 except the current number
      if (j == i)
        array2d[i][j] = 1;
      else
        array2d[i][j] = 0;
    }
  }
  return array2d;
}

double **init2dArray(size_t s1, size_t s2)
{
  // Initialize a 2D-array of size s1 * s2
  double **array2d = malloc(s1 * sizeof(double *));
  if (array2d == NULL)
  {
    fprintf(stderr, "Error: malloc failed\n");
  }
  for (size_t i = 0; i < s1; i++)
  {
    array2d[i] = malloc(s2 * sizeof(double));
    if (array2d[i] == NULL)
    {
      errx(1, "malloc failed");
    }
  }
  return array2d;
}

double ***init3dArray(size_t s1, size_t s2, size_t s3)
{
  // malloc triple array
  double ***trainingInputs = malloc(s1 * sizeof(double **));
  if (trainingInputs == NULL)
    errx(1,
         "initTrainingInputs: Could not allocate memory for trainingInputs");

  // malloc double array
  for (size_t i = 0; i < s1; i++)
  {
    trainingInputs[i] = malloc(s2 * sizeof(double *));
    if (trainingInputs[i] == NULL)
      errx(1,
           "initTrainingInputs: Could not allocate memory for "
           "trainingInputs[%zu]",
           i);
  }

  // malloc single array

  for (size_t i = 0; i < s1; i++)
  {
    for (size_t j = 0; j < s2; j++)
    {
      trainingInputs[i][j] = malloc(s3 * sizeof(double));
      if (trainingInputs[i][j] == NULL)
        errx(1,
             "initTrainingInputs: Could not allocate memory for "
             "trainingInputs[%zu][%zu]",
             i, j);
    }
  }

  return trainingInputs;
}

size_t countFolders(char *folderRoot)
{
  // Create a directory stream & initialize the counter
  struct dirent *entry;
  size_t         count = 0;

  // Open the given folder path
  DIR *dir = opendir(folderRoot);
  if (dir == NULL)
    errx(1, "countFolder: Could not open folder %s", folderRoot);

  // Count the number of files in the folder
  while ((entry = readdir(dir)) != NULL)
  {
    // Ignore the current & parent directories
    if (entry->d_name[0] != '.') // not safe, could also count regular files
      count++;
  }

  // Close the directory stream
  closedir(dir);

  // Return the number of training sets (folders) contained within the folder
  return count;
}
