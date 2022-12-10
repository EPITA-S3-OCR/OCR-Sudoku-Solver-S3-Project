#include "main.h"

// A comparison function used by qsort
int compare_filenames(const void *a, const void *b)
{
  const char *pa = *(const char **)a;
  const char *pb = *(const char **)b;

  return strcmp(pa, pb);
}

int ocrNeuralNetworkMain(int argc, char *argv[])
{
  // Generate random seed
  srand(time(NULL));

  // Prevent wrong number of arguments
  if (argc < 2)
    errx(1, "Usage: %s --option [...]\n", argv[0]);

  // Select the type of neural network to train
  if (!strcmp(argv[1], "--train"))
  {
    // Prevent wrong number of arguments
    if (argc != 5)
      errx(1, "Usage: %s --train nbEpochs imagesFoldersPath saveOutputPath\n",
           argv[0]);

    // Count the number of epochs to be imported in the given root folder
    char *subfolderPath = malloc(MAX_PATH_LENGTH);
    sprintf(subfolderPath, "%s", argv[3]);
    size_t TRAINING_SETS = countFolders(subfolderPath);

    // Initialize the training inputs
    double ***trainingInputs
        = init3dArray(TRAINING_SETS, TRAINING_SIZE, INPUT_SIZE);
    if (trainingInputs == NULL)
      errx(1, "ocrNeuralNetworkMain: Could not allocate memory for training "
              "inputs");
    for (size_t i = 0; i < TRAINING_SETS; i++)
    {
      for (size_t j = 1; j <= TRAINING_SIZE; j++)
      {
        char path[MAX_PATH_LENGTH];
        sprintf(path, "%s/%zu/%zu.jpg", subfolderPath, i, j);
        double *image = loadImage(path);
        for (int k = 0; k < INPUT_SIZE; k++)
          trainingInputs[i][j - 1][k] = image[k];
        free(image);
      }
    }

    // Free the subfolder path
    free(subfolderPath);

    // Initialize the training outputs as an identity matrix
    double **trainingOutputs = createIdentityMatrix(TRAINING_SIZE);

    // Initialize the training indexes array
    size_t *trainingIndexes = calloc(TRAINING_SIZE, sizeof(size_t));
    for (size_t i = 0; i < TRAINING_SIZE; i++)
      trainingIndexes[i] = i;

    // Initialize constant neural network parameters
    const double  learningRate = LEARNING_RATE;
    unsigned long maxEpochs    = strtoul(argv[2], NULL, 10);

    // Initialize the neural network
    NeuralNetwork nn;
    neuralNetworkInit(&nn, INPUT_SIZE, HIDDEN_SIZE, OUTPUT_SIZE, TRAINING_SIZE,
                      TRAINING_SETS);

    // Train the neural network
    neuralNetworkTrain(&nn, trainingInputs, trainingOutputs, trainingIndexes,
                       learningRate, maxEpochs);

    // Print weights & biases values after 'maxEpochs' trains
    neuralNetworkPrintResults(&nn, maxEpochs);

    // Save the neural network weights & biases to a file
    const char *ocrFN = argv[4];
    neuralNetworkSaveOCR(&nn, ocrFN);

    // Free the neural network
    neuralNetworkFree(&nn);
  }
  else if (!strcmp(argv[1], "--comp"))
  {
    // Prevent wrong number of arguments
    if (argc != 5)
      errx(1, "Usage: %s --comp savePath imagePath expectedValue\n", argv[0]);

    // Load the neural network weights & biases from a file
    NeuralNetwork nn;
    neuralNetworkLoadOCR(&nn, argv[2]);

    // Load the image to test
    char *imagePath = malloc(sizeof(char) * MAX_PATH_LENGTH);
    if (imagePath == NULL)
      errx(1, "%s: malloc failed\n", __func__);

    sprintf(imagePath, "%s", argv[3]);
    double *pixels = loadImage(imagePath);

    // Compute the neural network output & get expected output
    int output   = neuralNetworkCompute(&nn, pixels);
    int expected = atoi(argv[4]);

    // Print the result
    printf("\n=================\nNeural network yields %d ", output);
    if (output == expected)
    {
      printf("\033[0;32m");
      printf("OK\n");
      printf("\033[0m");
    }
    else
    {
      printf("\033[0;31m");
      printf("KO\n");
      printf("\033[0m");
    }

    // Free the neural network structure, its arrays & the image pixels / path
    free(pixels);
    free(imagePath);
    neuralNetworkFree(&nn);
  }
  else if (!strcmp(argv[1], "--train-from"))
  {
    // Prevent wrong number of arguments
    if (argc != 6)
      errx(1,
           "Usage: %s --train-from nbEpochs trainingFolder brainPath "
           "newBrainPath\n",
           argv[0]);

    // Load the neural network weights & biases from a file
    const char   *ocrFN = argv[4];
    NeuralNetwork nn;
    neuralNetworkLoadOCR(&nn, ocrFN);

    // Count the number of epochs to be imported in the given root folder
    char *subfolderPath = malloc(MAX_PATH_LENGTH);
    sprintf(subfolderPath, "%s", argv[3]);
    size_t TRAINING_SETS = countFolders(subfolderPath);

    // Initialize the training inputs
    double ***trainingInputs
        = init3dArray(TRAINING_SETS, TRAINING_SIZE, INPUT_SIZE);
    if (trainingInputs == NULL)
      errx(1, "ocrNeuralNetworkMain: Could not allocate memory for training "
              "inputs");
    for (size_t i = 0; i < TRAINING_SETS; i++)
    {
      for (size_t j = 1; j <= TRAINING_SIZE; j++)
      {
        char path[MAX_PATH_LENGTH];
        sprintf(path, "%s/%zu/%zu.jpg", subfolderPath, i, j);
        double *image = loadImage(path);
        for (int k = 0; k < INPUT_SIZE; k++)
          trainingInputs[i][j - 1][k] = image[k];
        free(image);
      }
    }

    // Free the subfolder path
    free(subfolderPath);

    // Initialize the training outputs as an identity matrix
    double **trainingOutputs = createIdentityMatrix(OUTPUT_SIZE);

    // Initialize the training indexes array
    size_t *trainingIndexes = calloc(TRAINING_SIZE, sizeof(size_t));
    for (size_t i = 0; i < TRAINING_SIZE; i++)
      trainingIndexes[i] = i;

    // Initialize constant neural network parameters
    const double  learningRate = LEARNING_RATE;
    unsigned long maxEpochs    = strtoul(argv[2], NULL, 10);

    // Train the neural network
    neuralNetworkTrain(&nn, trainingInputs, trainingOutputs, trainingIndexes,
                       learningRate, maxEpochs);

    // Print weights & biases values after 'maxEpochs' trains
    neuralNetworkPrintResults(&nn, maxEpochs);

    // Save the neural network weights & biases to a file
    const char *saveOcrFN = argv[5];
    neuralNetworkSaveOCR(&nn, saveOcrFN);

    // Free the neural network
    neuralNetworkFree(&nn);
  }
  else if (!strcmp(argv[1], "--comp-batch"))
  {
    // Prevent wrong number of arguments
    if (argc != 4)
      errx(1, "Usage: %s --comp-batch savePath folderName\n", argv[0]);

    // Load the neural network weights & biases from a file
    NeuralNetwork nn;
    neuralNetworkLoadOCR(&nn, argv[2]);

    DIR           *dir;
    struct dirent *entry;
    int            file_count = 0;
    char         **filenames  = NULL;

    // Open the directory
    dir = opendir(argv[3]);
    if (!dir)
    {
      printf("Error opening directory");
      return -1;
    }

    double **sudoku = init2dArray(9, 9);

    // Read entries from the directory and store them in an array
    while ((entry = readdir(dir)) != NULL)
    {
      // If the entry is a regular file and its it not from the banned list
      // [".", "..", ".*"], add its name to the array
      if (entry->d_type == DT_REG && entry->d_name[0] != '.')
      {
        // printf("File: %s\n", entry->d_name);
        filenames = realloc(filenames, sizeof(char *) * (file_count + 1));
        filenames[file_count] = malloc(strlen(entry->d_name) + 1);
        strcpy(filenames[file_count], entry->d_name);
        file_count++;
      }
    }

    // Close the directory
    closedir(dir);

    // Sort the array of filenames using qsort
    qsort(filenames, file_count, sizeof(char *), compare_filenames);

    for (int i = 0; i < file_count; i++)
    {
      // If the entry is a regular file, print its name
      char *imagePath = malloc(sizeof(char) * MAX_PATH_LENGTH);
      if (imagePath == NULL)
        errx(1, "%s: malloc failed\n", __func__);

      strcat(imagePath, argv[3]);
      strcat(imagePath, "/");
      strcat(imagePath, filenames[i]);

      // Load the image to test
      double *pixels = loadImage(imagePath);

      double whitePixelCount = 0;
      for (int i = 0; i < INPUT_SIZE; i++)
        if (pixels[i] == 1)
          whitePixelCount++;

      if (whitePixelCount < 10)
      {
        printf("Image %s is empty\n", imagePath);
        continue;
        sudoku[i / 9][i % 9] = 0;
      }

      // Compute the neural network output & get expected output
      int output           = neuralNetworkCompute(&nn, pixels);
      sudoku[i / 9][i % 9] = output;
      free(pixels);
      // Print the result
      printf("%s yields %d\n", filenames[i], output);
    }

    // Free the memory used by the array
    for (int i = 0; i < file_count; i++)
    {
      free(filenames[i]);
    }
    free(filenames);

    // print sudoku
    for (int i = 0; i < 9; i++)
    {
      for (int j = 0; j < 9; j++)
      {
        if (sudoku[i][j] == 0)
          printf(" .");
        else
          printf(" %d", (int)sudoku[i][j]);
        if (j % 3 == 2)
          printf(" ");
      }
      printf("\n");
      if (i % 3 == 2)
        printf("\n");
    }
    free(sudoku);
    // // Close the directory
    // closedir(dir);

    neuralNetworkFree(&nn);
  }
  else if (!strcmp(argv[1], "--train-from"))
  {
    // Prevent wrong number of arguments
    if (argc != 6)
      errx(1,
           "Usage: %s --train-from nbEpochs trainingFolder brainPath "
           "newBrainPath\n",
           argv[0]);

    // Load the neural network weights & biases from a file
    const char   *ocrFN = argv[4];
    NeuralNetwork nn;
    neuralNetworkLoadOCR(&nn, ocrFN);

    // Count the number of epochs to be imported in the given root folder
    char *subfolderPath = malloc(MAX_PATH_LENGTH);
    sprintf(subfolderPath, "%s", argv[3]);
    size_t TRAINING_SETS = countFolders(subfolderPath);

    // Initialize the training inputs
    double ***trainingInputs
        = init3dArray(TRAINING_SETS, TRAINING_SIZE, INPUT_SIZE);
    if (trainingInputs == NULL)
      errx(1, "ocrNeuralNetworkMain: Could not allocate memory for training "
              "inputs");
    for (size_t i = 0; i < TRAINING_SETS; i++)
    {
      for (size_t j = 1; j <= TRAINING_SIZE; j++)
      {
        char path[MAX_PATH_LENGTH];
        sprintf(path, "%s/%zu/%zu.jpg", subfolderPath, i, j);
        double *image = loadImage(path);
        for (int k = 0; k < INPUT_SIZE; k++)
          trainingInputs[i][j - 1][k] = image[k];
        free(image);
      }
    }

    // Free the subfolder path
    free(subfolderPath);

    // Initialize the training outputs as an identity matrix
    double **trainingOutputs = createIdentityMatrix(OUTPUT_SIZE);

    // Initialize the training indexes array
    size_t *trainingIndexes = calloc(TRAINING_SIZE, sizeof(size_t));
    for (size_t i = 0; i < TRAINING_SIZE; i++)
      trainingIndexes[i] = i;

    // Initialize constant neural network parameters
    const double  learningRate = LEARNING_RATE;
    unsigned long maxEpochs    = strtoul(argv[2], NULL, 10);

    // Train the neural network
    neuralNetworkTrain(&nn, trainingInputs, trainingOutputs, trainingIndexes,
                       learningRate, maxEpochs);

    // Print weights & biases values after 'maxEpochs' trains
    neuralNetworkPrintResults(&nn, maxEpochs);

    // Save the neural network weights & biases to a file
    const char *saveOcrFN = argv[5];
    neuralNetworkSaveOCR(&nn, saveOcrFN);

    // Free the neural network
    neuralNetworkFree(&nn);
  }
  else
  {
    errx(1, "Unknown option: %s", argv[1]);
    return 1;
  }

  return 0;
}
