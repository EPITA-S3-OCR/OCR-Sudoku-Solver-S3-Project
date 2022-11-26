#include "main.h"

int ocrNeuralNetworkMain(int argc, char *argv[])
{
  printf("%s", argv[1]);
  float *pixels = loadImage(argv[1]);
  printPixels(pixels);
  return 0;

  // Prevent wrong number of arguments
  if (argc < 2)
    errx(1, "Usage: %s --option [...]\n", argv[0]);

  // Select the type of neural network to train
  if (!strcmp(argv[1], "--train-ocr"))
  {
    // Prevent wrong number of arguments
    if (argc != 4)
      errx(1, "Usage: %s --train-ocr nbEpochs numbersFolderPath outputPath\n",
           argv[0]); // for now, have to be scaled

    // Generate random seed from current time
    srand(time(NULL));

    // numbersFolderPath : ["1", "2", "3", "4", "5", "6", "7", "8", "9"] images
    // epoch : a batch of the 9 pictures of each number
    // shuffle the array originally

    // Initialize the training inputs/outputs arrays

    double trainingInputs[TRAINING_SIZE]
                         [INPUT_SIZE]; // titouan's function will be used here
    double trainingOutputs[TRAINING_SIZE]
                          [OUTPUT_SIZE]; // maybe 10 for empty tile if not
                                         // dectected using image processing

    // Initialize the training indexes array
    size_t trainingIndexes[TRAINING_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    // Initialize constant neural network parameters
    const double  learningRate = .1f;
    unsigned long maxEpochs    = strtoul(argv[2], NULL, 10);

    // Initialize the neural network
    NeuralNetwork nn;
    neuralNetworkInit(&nn, INPUT_SIZE, HIDDEN_SIZE, OUTPUT_SIZE,
                      TRAINING_SIZE);

    // Train the neural network
    neuralNetworkTrain(&nn, trainingInputs, trainingOutputs, trainingIndexes,
                       learningRate, maxEpochs);

    // Print weights & biases values after 'maxEpochs' trains
    neuralNetworkPrintResults(&nn, maxEpochs);

    // Save the neural network weights & biases to a file
    const char *ocrFN = argv[4];
    neuralNetworkSaveOCR(&nn, ocrFN);

    // Free the neural network & its arrays
    neuralNetworkFree(&nn);
  }
  else
  {
    errx(1, "Unknown option: %s", argv[1]);
    return 1;
  }

  return 0;
}
