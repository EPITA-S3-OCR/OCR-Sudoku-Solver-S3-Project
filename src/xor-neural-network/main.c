#include "main.h"

int xorNeuralNetworkMain(int argc, char *argv[])
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
    // Prevent larger than 8 bit words
    if (strlen(argv[3]) > 8 || strlen(argv[4]) > 8)
      errx(1, "Usage: %s --comp-xor relPath a b\n", argv[0]);

    // Prevent wrong number of arguments
    if (argc != 5)
      errx(1, "Usage: %s --comp-xor relPath a b\n", argv[0]);

    // Load the neural network weights & biases from a file
    const char   *xorFN = argv[2];
    NeuralNetwork nn;
    neuralNetworkLoadXOR(&nn, xorFN);

    // Retrieve the two numbers to compute the XOR
    int nb1 = atoi(argv[3]), nb2 = atoi(argv[4]);

    // Convert the inputs to binary
    int bin2 = getDecimalInt(nb2), bin1 = getDecimalInt(nb1);

    double inputs[INPUT_SIZE] = {(double)bin1, (double)bin2};

    // Compute the neural network output & print it
    int output = neuralNetworkComputeMultiple(&nn, inputs);

    printf("\n=================\nNeural network yields: %s ^ %s = %s ",
           getBinaryString(getDecimalInt(nb1)),
           getBinaryString(getDecimalInt(nb2)), getBinaryString(output));
    if (output == ((int)inputs[0] ^ (int)inputs[1]))
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
    neuralNetworkFree(&nn);
  }
  else if (!strcmp(argv[1], "--train-from"))
  {
    // Prevent wrong number of arguments
    if (argc != 5)
      errx(1, "Usage: %s --train-from nbEpochs relPath1 relPath2\n", argv[0]);

    // Load the neural network weights & biases from the first given file
    const char   *xorFN = argv[3];
    NeuralNetwork nn;
    neuralNetworkLoadXOR(&nn, xorFN);

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

    // Train the neural network loaded from the first given file
    neuralNetworkTrain(&nn, trainingInputs, trainingOutputs, trainingIndexes,
                       learningRate, maxEpochs);

    printf("finished loading & setting up\n");

    // Save the neural network weights & biases to the second given file
    const char *xorFN2 = argv[4];
    neuralNetworkSaveXOR(&nn, xorFN2);

    // Print weights & biases values after 'maxEpochs' trains
    neuralNetworkPrintResults(&nn, maxEpochs);

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
