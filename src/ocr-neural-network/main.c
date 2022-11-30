#include "main.h"

int ocrNeuralNetworkMain(int argc, char *argv[])
{
  // Prevent wrong number of arguments
  if (argc < 2)
    errx(1, "Usage: %s --option [...]\n", argv[0]);

  // Select the type of neural network to train
  if (!strcmp(argv[1], "--train-ocr"))
  {
    // Prevent wrong number of arguments
    if (argc != 5)
      errx(1, "Usage: %s --train-ocr nbEpochs numbersFolderPath outputPath\n",
           argv[0]); // for now, have to be scaled

    // Generate random seed from current time
    srand(time(NULL));

    // shuffle the array originally

    double trainingInputs[TRAINING_SIZE][INPUT_SIZE];

    // Initialize the training inputs
    for (size_t i = 0; i < TRAINING_SIZE; i++)
    {
      // Load the matrix of pixels from the given images
      char *path = malloc(sizeof(char) * MAX_PATH_LENGTH);
      sprintf(path, "%s/%zu.png", argv[3], i + 1);
      float *pixels = loadImage(path);

      // Load every pixel value in the current training input
      for (size_t j = 0; j < INPUT_SIZE; j++)
        trainingInputs[i][j] = pixels[j];

      // Free the memory allocated for the pixels & path
      free(pixels);
      free(path);
    }

    double trainingOutputs[TRAINING_SIZE][OUTPUT_SIZE];

    // Initialize the training outputs
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
    size_t trainingIndexes[TRAINING_SIZE] = {0, 1, 2}; //, 3, 4, 5, 6, 7, 8};

    // Initialize constant neural network parameters
    const double  learningRate = .8f; //.1f;
    unsigned long maxEpochs    = strtoul(argv[2], NULL, 10);

    // Initialize the neural network
    NeuralNetwork nn;
    neuralNetworkInit(&nn, INPUT_SIZE, HIDDEN_SIZE, OUTPUT_SIZE,
                      TRAINING_SIZE);

    // Train the neural network
    neuralNetworkTrain(&nn, trainingInputs, trainingOutputs, trainingIndexes,
                       learningRate, maxEpochs);
    print1dArray(nn.outputLayer, nn.nbOutputNeurons);
    // Print weights & biases values after 'maxEpochs' trains
    neuralNetworkPrintResults(&nn, maxEpochs);

    print1dArray(nn.outputLayer, nn.nbOutputNeurons);
    // Save the neural network weights & biases to a file
    const char *ocrFN = argv[4];

    neuralNetworkSaveOCR(&nn, ocrFN);

    // // Test with a random image
    // char  *path        = malloc(sizeof(char) * MAX_PATH_LENGTH);
    // size_t randomIndex = rand() % TRAINING_SIZE;
    // sprintf(path, "tests/ocr/tiles/3/2.png");
    // // sprintf(path, "tests/ocr/tiles/simple_epoch/%zu.png", randomIndex +
    // 1);
    // // sprintf(path, "%s/%zu.png", argv[3], randomIndex + 1);
    // printf("Testing with a random image (%zu)...\n", randomIndex + 1);
    // float *pixels = loadImage(path);
    // print1dArray(nn.outputLayer, nn.nbOutputNeurons);
    // neuralNetworkCompute(&nn, pixels, 3);
    // free(pixels);
    // free(path);
    // Free the neural network & its arrays
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

    // printf("here\n");
    // printf("argv[0] : %s\n", argv[0]);
    // printf("argv[3] : %s\n", argv[3]);

    char *imagePath = malloc(sizeof(char) * MAX_PATH_LENGTH);
    sprintf(imagePath, "%s", argv[3]);

    printf("Testing with a random image (%s)...\n", imagePath);
    float *pixels   = loadImage(imagePath);
    int    output   = neuralNetworkCompute(&nn, pixels);
    int    expected = atoi(argv[4]);
    print1dArray(nn.outputLayer, nn.nbOutputNeurons);

    printf("\n=================\nNeural network yields %d", output);
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

    printf("\033[0m");
    printf("\033[0m");

    // Free the neural network & its arrays
    free(pixels);
    free(imagePath);
    // free(ocrFN);
    neuralNetworkFree(&nn);
  }
  else
  {
    errx(1, "Unknown option: %s", argv[1]);
    return 1;
  }

  return 0;
}
