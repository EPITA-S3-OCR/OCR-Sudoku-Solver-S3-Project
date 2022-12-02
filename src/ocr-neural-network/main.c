#include "main.h"

// float *loadRandomImage(char *folder_path, int *digit)
// {
//   int random_number = rand() % 9;

//   char path[MAX_PATH_LENGTH];
//   sprintf(path, "%s/%d/%d.png", folder_path, digit, random_number);

//   return loadImage(path);
// }

// void loadAllInputs(double trainingInputs[TRAINING_SIZE][INPUT_SIZE],
//                    int *expected, char *folder_path)

// {
//   for (int i = 0; i < TRAINING_SIZE; i++)
//   {
//     // generate a random number
//     int    random_number = rand() % 9;
//     float *image         = loadRandomImage(folder_path, random_number);
//     for (int j = 0; j < INPUT_SIZE; j++)
//     {
//       trainingInputs[i][j] = image[j];
//     }
//     expected[i] = random_number;
//   }
// }

size_t countTrainingSets(char *folderRoot)
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

double ***initTrainingInputs(size_t trainingSets, size_t trainingSize,
                             size_t inputSize)
{
  // malloc triple array
  double ***trainingInputs = malloc(trainingSets * sizeof(double **));
  if (trainingInputs == NULL)
    errx(1,
         "initTrainingInputs: Could not allocate memory for trainingInputs");

  // malloc double array
  for (size_t i = 0; i < trainingSets; i++)
  {
    trainingInputs[i] = malloc(trainingSize * sizeof(double *));
    if (trainingInputs[i] == NULL)
      errx(1,
           "initTrainingInputs: Could not allocate memory for "
           "trainingInputs[%zu]",
           i);
  }

  // malloc single array

  for (size_t i = 0; i < trainingSets; i++)
  {
    for (size_t j = 0; j < trainingSize; j++)
    {
      trainingInputs[i][j] = malloc(inputSize * sizeof(double));
      if (trainingInputs[i][j] == NULL)
        errx(1,
             "initTrainingInputs: Could not allocate memory for "
             "trainingInputs[%zu][%zu]",
             i, j);
    }
  }

  return trainingInputs;
}

int ocrNeuralNetworkMain(int argc, char *argv[])
{
  // Generate random seed
  srand(time(NULL));

  // Prevent wrong number of arguments
  if (argc < 2)
    errx(1, "Usage: %s --option [...]\n", argv[0]);

  // Select the type of neural network to train
  if (!strcmp(argv[1], "--train-ocr"))
  {
    // Prevent wrong number of arguments
    if (argc != 5)
      errx(1,
           "Usage: %s --train-ocr nbEpochs imagesFoldersPath saveOutputPath\n",
           argv[0]);

    // Count the number of epochs to be imported in the given root folder
    char *subfolderPath = malloc(MAX_PATH_LENGTH);
    sprintf(subfolderPath, "%s", argv[3]);
    size_t TRAINING_SETS = countTrainingSets(subfolderPath);

    // Initialize the training inputs
    double ***trainingInputs
        = initTrainingInputs(TRAINING_SETS, TRAINING_SIZE, INPUT_SIZE);
    if (trainingInputs == NULL)
      errx(1, "ocrNeuralNetworkMain: Could not allocate memory for training "
              "inputs");
    for (size_t i = 0; i < TRAINING_SETS; i++)
    {
      printf("%zu\n", i);
      for (size_t j = 1; j <= TRAINING_SIZE; j++)
      {
        printf("%zu\n", j);
        char path[MAX_PATH_LENGTH];
        sprintf(path, "%s/%zu/%zu.png", subfolderPath, i, j);
        double *image = loadImage(path);
        for (int k = 0; k < INPUT_SIZE; k++)
          trainingInputs[i][j - 1][k] = image[k];
        free(image);
      }
    }

    // for (size_t i = 0; i < TRAINING_SETS; i++)
    // {
    //   for (size_t j = 0; j < TRAINING_SIZE; j++)
    //   {
    //     printPixels(trainingInputs[i][j]);
    //     printf("\n");
    //   }
    // }

    // Initialize the training outputs
    double trainingOutputs[TRAINING_SIZE][OUTPUT_SIZE];
    for (size_t i = 0; i < TRAINING_SIZE; i++)
    {
      // Initialize the current training output
      for (size_t j = 0; j < OUTPUT_SIZE; j++)
      {
        // Everything should be 0 except the current number
        if (j == i)
          trainingOutputs[i][j] = 1;
        else
          trainingOutputs[i][j] = 0;
      }
    }

    // Initialize the training indexes array
    size_t trainingIndexes[TRAINING_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    // Initialize constant neural network parameters
    const double  learningRate = .1f;
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
  else if (!strcmp(argv[1], "--comp-ocr"))
  {
    // Prevent wrong number of arguments
    if (argc != 5)
      errx(1, "Usage: %s --comp-ocr brainPath imagePath expected\n", argv[0]);

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
  else
  {
    errx(1, "Unknown option: %s", argv[1]);
    return 1;
  }

  return 0;
}
