// Import librairies & helper files
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "neural_network.h"

#define INPUT_SIZE 2
#define HIDDEN_SIZE 2
#define OUTPUT_SIZE 1
#define TRAINING_SIZE 4

int main(int argc, char *argv[])
{
  // Prevent wrong number of arguments
  if (argc < 2)
    errx(1, "Usage: %s --option [...]\n", argv[0]);

  // Select the type of neural network to train
  if (!strcmp(argv[1], "--train-xor"))
  {
    // Prevent wrong number of arguments
    if (argc != 4)
      errx(1, "Usage: %s --train-xor nbEpochs relPath\n", argv[0]);

    // Generate random seed from current time
    srand(time(NULL));

    // Initialize the training inputs/outputs arrays
    double trainingInputs[TRAINING_SIZE][INPUT_SIZE]
        = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double trainingOutputs[TRAINING_SIZE][OUTPUT_SIZE] = {{0}, {1}, {1}, {0}};

    // Initialize the training indexes array
    size_t trainingIndexes[TRAINING_SIZE] = {0, 1, 2, 3};

    // Initialize constant neural network parameters
    const double  learningRate = 0.1f;
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
    const char *xorFN = argv[3];
    neuralNetworkSaveXOR(&nn, xorFN);

    // Free the neural network & its arrays
    neuralNetworkFree(&nn);
  }
  else if (!strcmp(argv[1], "--comp-xor"))
  {
    // Prevent wrong number of arguments
    if (argc != 5)
      errx(1, "Usage: %s --comp-xor relPath a b\n", argv[0]);

    // Load the neural network weights & biases from a file
    const char   *xorFN = ;
    NeuralNetwork nn;
    neuralNetworkLoadXOR(&nn, xorFN);

    // Initialize the inputs array
    double inputs[INPUT_SIZE] = {strtod(argv[3], NULL), strtod(argv[4], NULL)};

    // Compute the neural network output & print it
    int output = (int)round(neuralNetworkCompute(&nn, inputs));
    printf("\n=================\nNeural network yields: %d ^ %d = %d ⇒  ",
           (int)inputs[0], (int)inputs[1], output);

    if (output == ((int)inputs[0] ^ (int)inputs[1]))
      printf("✅\n");
    else
      printf("❌\n");

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