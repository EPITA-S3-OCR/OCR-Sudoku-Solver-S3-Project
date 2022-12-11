#include "main.h"

void train(char *subfolderPath, unsigned long maxEpochs, char *outputPath,
           bool verbose)
{ // Count the number of epochs to be imported in the given root folder
  // char *subfolderPath = malloc(MAX_PATH_LENGTH);
  // sprintf(subfolderPath, "%s", argv[3]);
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
  // free(subfolderPath);

  // Initialize the training outputs as an identity matrix
  double **trainingOutputs = createIdentityMatrix(TRAINING_SIZE);

  // Initialize the training indexes array
  size_t *trainingIndexes = calloc(TRAINING_SIZE, sizeof(size_t));
  for (size_t i = 0; i < TRAINING_SIZE; i++)
    trainingIndexes[i] = i;

  // Initialize constant neural network parameters
  const double learningRate = LEARNING_RATE;
  // unsigned long maxEpochs    = strtoul(argv[2], NULL, 10);

  // Initialize the neural network
  NeuralNetwork nn;
  neuralNetworkInit(&nn, INPUT_SIZE, HIDDEN_SIZE, OUTPUT_SIZE, TRAINING_SIZE,
                    TRAINING_SETS);

  // Train the neural network
  neuralNetworkTrain(&nn, trainingInputs, trainingOutputs, trainingIndexes,
                     learningRate, maxEpochs, verbose);

  // Print weights & biases values after 'maxEpochs' trains
  // neuralNetworkPrintResults(&nn, maxEpochs);
  printf("Success rate for %zu epochs: %lf\n", maxEpochs,
         (nn.successCount / nn.totalTries) * 100);
  printf("Verbose : %s\n", verbose ? "true" : "false");
  if (verbose)
  {
    char *str = malloc(100 * sizeof(char));
    sprintf(str, "📊 Success rate for %zu epochs: %.2lf%%\n", maxEpochs,
            (nn.successCount / nn.totalTries) * 100);

    g_idle_add(addConsoleMessage, str);
  }
  // Save the neural network weights & biases to a file
  // const char *ocrFN = argv[4];
  const char *ocrFN = outputPath;
  neuralNetworkSaveOCR(&nn, ocrFN);

  // Free the neural network
  neuralNetworkFree(&nn);
}

// A comparison function used by qsort
int compare_filenames(const void *a, const void *b)
{
  const char *pa = *(const char **)a;
  const char *pb = *(const char **)b;

  return strcmp(pa, pb);
}
void ocrNeuralNetworkUi(int epoch, bool verbose)
{
  train("tests/ocr/tiles", epoch, "output/train/train_live.txt", verbose);
}

void compare(char *ocrPath, char *compareDirPath)
{

  // Load the neural network weights & biases from a file
  NeuralNetwork nn;
  neuralNetworkLoadOCR(&nn, ocrPath);
  int            size = 9;
  DIR           *dir;
  struct dirent *entry;
  int            file_count = 0;
  char         **filenames  = NULL;

  // Open the directory
  dir = opendir(compareDirPath);
  if (!dir)
  {
    printf("Error opening directory");
    return;
  }

  // double **sudoku = init2dArray(16, 16);

  char sudoku[16][16];

  // Read entries from the directory and store them in an array
  while ((entry = readdir(dir)) != NULL)
  {
    // If the entry is a regular file and its it not from the banned list
    // [".", "..", ".*"], add its name to the array
    if (entry->d_name[0] != '.')
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

    // strcat(imagePath, compareDirPath);
    // strcat(imagePath, "/");
    // strcat(imagePath, filenames[i]);
    sprintf(imagePath, "%s/%s", compareDirPath, filenames[i]);
    // Load the image to test
    double *pixels = loadImage(imagePath);

    double whitePixelCount = 0;
    for (int i = 0; i < INPUT_SIZE; i++)
      if (pixels[i] == 1)
        whitePixelCount++;

    if (whitePixelCount < 10)
    {
      printf("Image %s is empty\n", imagePath);
      sudoku[i / size][i % size] = '.';
      continue;
      // sudoku[i / size][i % size] = 0;
    }

    // Compute the neural network output & get expected output
    int   output    = neuralNetworkCompute(&nn, pixels);
    char *outputStr = malloc(sizeof(char) * 2);
    if (outputStr == NULL)
      errx(1, "%s: malloc failed\n", __func__);
    sprintf(outputStr, "%d", output);
    sudoku[i / size][i % size] = outputStr[0];
    printf("suoku[%d][%d] = %s\n", i / size, i % size, outputStr);
    free(pixels);
    // Print the result
    printf("%s yields %d -> %s\n", filenames[i], output, outputStr);
  }
  // Free the memory used by the array
  for (int i = 0; i < file_count; i++)
  {
    free(filenames[i]);
  }
  free(filenames);
  printf("here\n");
  // Print sudoku
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      printf("%c ", sudoku[i][j]);
    }
    printf("\n");
  }

  createFile(sudoku, "output/ui/sudoku", size);

  // free(sudoku);
  neuralNetworkFree(&nn);
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
    // train("tests/ocr/tiles", 1000, "output/train/train_live.txt", true);
    // ocrNeuralNetworkUi(10, true);
    train(argv[3], strtoul(argv[2], NULL, 10), argv[4], false);
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
                       learningRate, maxEpochs, false);

    // Print weights & biases values after 'maxEpochs' trains
    // neuralNetworkPrintReosults(&nn, maxEpochs);

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

    compare(argv[2], argv[3]);
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
                       learningRate, maxEpochs, false);

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

void ocrUi()
{
  compare("output/train/train.txt", "output/tiles");
}