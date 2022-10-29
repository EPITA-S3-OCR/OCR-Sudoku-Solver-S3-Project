#include "neural_network.h"

void neuralNetworkInit(NeuralNetwork *nn, size_t nbInputNeurons,
                       size_t nbHiddenNeurons, size_t nbOutputNeurons,
                       size_t nbTraining)
{
  // Initialize the layer properties of the neural network
  nn->nbInputNeurons  = nbInputNeurons;
  nn->nbHiddenNeurons = nbHiddenNeurons;
  nn->nbOutputNeurons = nbOutputNeurons;
  nn->nbTraining      = nbTraining;

  // Initialize statistics
  nn->successCount = 0;
  nn->totalTries   = 0;

  // Allocate the hidden & output layer arrays
  nn->hiddenLayer = calloc(nbHiddenNeurons, sizeof(double));
  nn->outputLayer = calloc(nbOutputNeurons, sizeof(double));

  // Allocate memory & fill the hidden layers weights array
  for (size_t i = 0; i < nbInputNeurons; i++)
  {
    nn->hiddenWeights[i] = (double *)calloc(nbHiddenNeurons, sizeof(double));
    arrayFillUniform(nn->hiddenWeights[i], nbHiddenNeurons, 1);
  }

  // Allocate memory & fill the output layer weights array
  for (size_t i = 0; i < nbHiddenNeurons; i++)
  {
    nn->outputWeights[i] = (double *)calloc(nbOutputNeurons, sizeof(double));
    arrayFillUniform(nn->outputWeights[i], nbOutputNeurons, 1);
  }

  // Allocate memory & fill the hidden layers biases arrays
  nn->hiddenBiases = calloc(nbHiddenNeurons, sizeof(double));
  arrayFillUniform(nn->hiddenBiases, nbHiddenNeurons, 1);

  // Allocate memory & fill the output layer biases arrays
  nn->outputBiases = calloc(nbOutputNeurons, sizeof(double));
  arrayFillUniform(nn->outputBiases, nbOutputNeurons, 1);
}

void neuralNetworkTrain(
    NeuralNetwork *nn,
    double         trainingInputs[nn->nbTraining][nn->nbInputNeurons],
    double         trainingOutputs[nn->nbTraining][nn->nbOutputNeurons],
    size_t trainingIndexes[nn->nbTraining], const double learningRate,
    unsigned long nbEpochs)
{
  // Process to train the neural network
  for (size_t epoch = 0; epoch < nbEpochs; epoch++)
  {
    // Shuffle the indexes to randomly train the current epoch
    arrayShuffle(trainingIndexes, nn->nbTraining);

    // Activate each layer in the shuffled order of the training indexes
    for (size_t x = 0; x < nn->nbTraining; x++)
    {
      // Grab the current shuffled index to activate on
      size_t i = trainingIndexes[x];

      /* ---- FORWARD PASS ---- */
      // Hidden layer activation
      for (size_t j = 0; j < nn->nbHiddenNeurons; j++)
      {
        double acvn = nn->hiddenBiases[j];

        for (size_t k = 0; k < nn->nbInputNeurons; k++)
          acvn += trainingInputs[i][k] * nn->hiddenWeights[k][j];
        nn->hiddenLayer[j] = sigmoid(acvn);
      }

      // Output layer activation
      for (size_t j = 0; j < nn->nbOutputNeurons; j++)
      {
        double acvn = nn->outputBiases[j];

        for (size_t k = 0; k < nn->nbHiddenNeurons; k++)
          acvn += nn->hiddenLayer[k] * nn->outputWeights[k][j];
        nn->outputLayer[j] = sigmoid(acvn);
      }

      // Print informations about current activation
      neuralNetworkPrintAssertXOR(nn, epoch, (int)trainingInputs[i][0],
                                  (int)trainingInputs[i][1]);

      /* ---- BACKPROPAGATION ---- */
      // Change in output weights
      double deltaOutput[nn->nbOutputNeurons];
      for (size_t j = 0; j < nn->nbOutputNeurons; j++)
      {
        double error   = trainingOutputs[i][j] - nn->outputLayer[j];
        deltaOutput[j] = error * sigmoidDerivative(nn->outputLayer[j]);
      }

      // Change in hidden weights
      double deltaHidden[nn->nbHiddenNeurons];
      for (size_t j = 0; j < nn->nbHiddenNeurons; j++)
      {
        double error = 0;
        for (size_t k = 0; k < nn->nbOutputNeurons; k++)
          error += deltaOutput[k] * (nn->outputWeights[j][k]);
        deltaHidden[j] = error * sigmoidDerivative(nn->hiddenLayer[j]);
      }

      // Apply the changes to the output layers weights
      for (size_t j = 0; j < nn->nbOutputNeurons; j++)
      {
        nn->outputBiases[j] += deltaOutput[j] * learningRate;

        for (size_t k = 0; k < nn->nbHiddenNeurons; k++)
          nn->outputWeights[k][j]
              += nn->hiddenLayer[k] * deltaOutput[j] * learningRate;
      }

      // Apply the changes to the hidden layers weights
      for (size_t j = 0; j < nn->nbHiddenNeurons; j++)
      {
        nn->hiddenBiases[j] += deltaHidden[j] * learningRate;

        for (size_t k = 0; k < nn->nbInputNeurons; k++)
          nn->hiddenWeights[k][j]
              += trainingInputs[i][k] * deltaHidden[j] * learningRate;
      }
    }
  }
}

void neuralNetworkPrintAssertXOR(NeuralNetwork *nn, unsigned long epoch, int a,
                                 int b)
{
  int expected = a ^ b;
  printf("Epoch %lu: %d ^ %d = %d ", epoch, a, b,
         (int)round(nn->outputLayer[0]));

  nn->totalTries++;
  if (round(nn->outputLayer[0]) == expected)
  {
    printf("✅\n");
    nn->successCount++;
  }
  else
    printf("❌\n");
}

void neuralNetworkPrintResults(NeuralNetwork *nn, size_t maxEpochs)
{
  printf("\n==== Hidden layers infos after %lu epochs ====\n", maxEpochs);
  printf("Hidden layer weights:\t");
  print2dArray(nn->hiddenWeights, nn->nbInputNeurons, nn->nbHiddenNeurons);
  printf("Hidden layer biases:\t");
  print1dArray(nn->hiddenBiases, nn->nbHiddenNeurons);

  printf("\n==== Output layers infos after %lu epochs ====\n", maxEpochs);
  printf("Output layer weights:\t");
  print2dArray(nn->outputWeights, nn->nbHiddenNeurons, nn->nbOutputNeurons);
  printf("Output layer biases:\t");
  print1dArray(nn->outputBiases, nn->nbOutputNeurons);

  printf("Success rate: %lf\n", (nn->successCount / nn->totalTries) * 100);
}

void neuralNetworkSaveXOR(NeuralNetwork *nn, const char *filename)
{
  // Open the file
  FILE *file = fopen(filename, "w");

  // Write the layers sizes
  fprintf(file, "%lu %lu %lu\n", nn->nbInputNeurons, nn->nbHiddenNeurons,
          nn->nbOutputNeurons);

  // Write the hidden layer weights
  for (size_t i = 0; i < nn->nbInputNeurons; i++)
  {
    for (size_t j = 0; j < nn->nbHiddenNeurons; j++)
      fprintf(file, "%lf\n", nn->hiddenWeights[i][j]);

    // Write the hidden layer biases
    fprintf(file, "%lf\n", nn->hiddenBiases[i]);
  }

  // Write the output layer weights
  for (size_t i = 0; i < nn->nbHiddenNeurons; i++)
  {
    for (size_t j = 0; j < nn->nbOutputNeurons; j++)
      fprintf(file, "%lf\n", nn->outputWeights[i][j]);
  }

  // Write the output layer biases
  fprintf(file, "%lf\n", nn->outputBiases[0]);

  // Close the file
  fclose(file);
}

void neuralNetworkLoadXOR(NeuralNetwork *nn, const char *filename)
{
  // Open the file
  FILE *file = fopen(filename, "r");

  printf("\n=================\nLoading XOR neural network from file %s...\n",
         filename);

  // Read the layers sizes
  fscanf(file, "%lu %lu %lu\n", &nn->nbInputNeurons, &nn->nbHiddenNeurons,
         &nn->nbOutputNeurons);
  printf("Loading XOR neural network with %lu input neurons, %lu hidden "
         "neurons and %lu output neurons\n",
         nn->nbInputNeurons, nn->nbHiddenNeurons, nn->nbOutputNeurons);

  // Allocate the memory spaces for the layers
  printf("Allocating memory for the layers...\n");
  nn->hiddenLayer = calloc(nn->nbHiddenNeurons, sizeof(double));
  nn->outputLayer = calloc(nn->nbOutputNeurons, sizeof(double));

  printf("Allocating memory for the biases...\n");
  nn->hiddenBiases = calloc(nn->nbHiddenNeurons, sizeof(double));
  nn->outputBiases = calloc(nn->nbOutputNeurons, sizeof(double));

  // Allocate memory & load the hidden weights & biases arrays
  printf("\nAllocating memory for the hidden layer weights & biases...\n");
  for (size_t i = 0; i < nn->nbInputNeurons; i++)
  {
    nn->hiddenWeights[i]
        = (double *)calloc(nn->nbHiddenNeurons, sizeof(double));

    for (size_t j = 0; j < nn->nbHiddenNeurons; j++)
    {
      fscanf(file, "%lf\n", &nn->hiddenWeights[i][j]);
      printf("Loading hidden weight [%lu, %lu] with value %lf\n", i, j,
             nn->hiddenWeights[i][j]);
    }
    fscanf(file, "%lf\n", &nn->hiddenBiases[i]);
    printf("Loading hidden bias [%lu] with value %lf\n", i,
           nn->hiddenBiases[i]);
  }

  // Allocate memory & load the hidden weights & biases arrays
  printf("\nAllocating memory for the ouptut layer weights & biases...\n");
  for (size_t i = 0; i < nn->nbHiddenNeurons; i++)
  {
    nn->outputWeights[i]
        = (double *)calloc(nn->nbOutputNeurons, sizeof(double));

    for (size_t j = 0; j < nn->nbOutputNeurons; j++)
    {
      fscanf(file, "%lf\n", &nn->outputWeights[i][j]);
      printf("Loading output weight [%lu, %lu] with value %lf\n", i, j,
             nn->outputWeights[i][j]);
    }
  }

  for (size_t i = 0; i < nn->nbOutputNeurons; i++)
  {
    fscanf(file, "%lf\n", &nn->outputBiases[i]);
    printf("Loading output bias [%lu] with value %lf\n", i,
           nn->outputBiases[i]);
  }
  // Close the file
  fclose(file);
}

int bitExtracted(int number, int k, int p)
{
  return (((1 << k) - 1) & (number >> (p - 1)));
}

int neuralNetworkComputeMultiple(NeuralNetwork *nn, double *inputs)
{
  int result = 0;
  for (int i = sizeof(int) * 2; i >= 0; i--)
  {
    int    nb1      = bitExtracted((int)inputs[0], 1, i);
    int    nb2      = bitExtracted((int)inputs[1], 1, i);
    double input[2] = {nb1, nb2};
    double res      = neuralNetworkCompute(nn, input);
    result += (int)round(res) * pow(2, i - 1);
  }
  return result;
}

double neuralNetworkCompute(NeuralNetwork *nn, double *inputs)
{
  // Compute the hidden layer
  for (size_t j = 0; j < nn->nbHiddenNeurons; j++)
  {
    double acvn = nn->hiddenBiases[j];
    for (size_t k = 0; k < nn->nbInputNeurons; k++)
      acvn += inputs[k] * nn->hiddenWeights[k][j];
    nn->hiddenLayer[j] = sigmoid(acvn);
  }

  // Compute the output layer
  for (size_t j = 0; j < nn->nbOutputNeurons; j++)
  {
    double acvn = nn->outputBiases[j];
    for (size_t k = 0; k < nn->nbHiddenNeurons; k++)
      acvn += nn->hiddenLayer[k] * nn->outputWeights[k][j];
    nn->outputLayer[j] = sigmoid(acvn);
  }

  // Return the result of the neural network
  return nn->outputLayer[0];
}

void neuralNetworkFree(NeuralNetwork *nn)
{
  // Free all the neural network's allocated memory spaces
  free(nn->hiddenLayer);
  free(nn->outputLayer);
  free2dArray(nn->hiddenWeights, nn->nbInputNeurons);
  free2dArray(nn->outputWeights, nn->nbHiddenNeurons);
  free(nn->hiddenBiases);
  free(nn->outputBiases);
}