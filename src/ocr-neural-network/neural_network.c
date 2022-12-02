#include "neural_network.h"

void neuralNetworkInit(NeuralNetwork *nn, size_t nbInputNeurons,
                       size_t nbHiddenNeurons, size_t nbOutputNeurons,
                       size_t nbTraining, size_t nbTrainingSets)
{
  // Initialize the layer properties of the neural network
  nn->nbInputNeurons  = nbInputNeurons;
  nn->nbHiddenNeurons = nbHiddenNeurons;
  nn->nbOutputNeurons = nbOutputNeurons;
  nn->nbTraining      = nbTraining;
  nn->nbTrainingSets  = nbTrainingSets;

  // Initialize statistics
  nn->successCount = 0;
  nn->totalTries   = 0;

  // Allocate the hidden & output layer arrays
  nn->hiddenLayer = calloc(nbHiddenNeurons, sizeof(double));
  if (nn->hiddenLayer == NULL)
    errx(1, "neuralNetworkInit: calloc failed for nn->hiddenLayer");

  nn->outputLayer = calloc(nbOutputNeurons, sizeof(double));
  if (nn->outputLayer == NULL)
    errx(1, "neuralNetworkInit: calloc failed for nn->outputLayer");

  // Allocate the hidden & output weights arrays
  nn->hiddenWeights = malloc(nbInputNeurons * sizeof(double *));
  if (nn->hiddenWeights == NULL)
    errx(1, "neuralNetworkInit: malloc failed for nn->hiddenWeights");

  nn->outputWeights = malloc(nbHiddenNeurons * sizeof(double *));
  if (nn->outputWeights == NULL)
    errx(1, "neuralNetworkInit: malloc failed for nn->outputWeights");

  // Allocate memory & fill the hidden layers weights array
  for (size_t i = 0; i < nbInputNeurons; i++)
  {
    nn->hiddenWeights[i] = (double *)calloc(nbHiddenNeurons, sizeof(double));
    if (nn->hiddenWeights[i] == NULL)
      errx(1, "neuralNetworkInit: calloc failed for nn->hiddenWeights[%zu]",
           i);
    arrayFillUniform(nn->hiddenWeights[i], nbHiddenNeurons, 1);
  }

  // Allocate memory & fill the output layer weights array
  for (size_t i = 0; i < nbHiddenNeurons; i++)
  {
    nn->outputWeights[i] = (double *)calloc(nbOutputNeurons, sizeof(double));
    if (nn->outputWeights[i] == NULL)
      errx(1, "neuralNetworkInit: calloc failed for nn->outputWeights[%zu]",
           i);
    arrayFillUniform(nn->outputWeights[i], nbOutputNeurons, 1);
  }

  // Allocate memory & fill the hidden layers biases arrays
  nn->hiddenBiases = calloc(nbHiddenNeurons, sizeof(double));
  if (nn->hiddenBiases == NULL)
    errx(1, "neuralNetworkInit: calloc failed for nn->hiddenBiases");
  arrayFillUniform(nn->hiddenBiases, nbHiddenNeurons, 1);

  // Allocate memory & fill the output layer biases arrays
  nn->outputBiases = calloc(nbOutputNeurons, sizeof(double));
  if (nn->outputBiases == NULL)
    errx(1, "neuralNetworkInit: calloc failed for nn->outputBiases");
  arrayFillUniform(nn->outputBiases, nbOutputNeurons, 1);
}

void neuralNetworkTrain(
    NeuralNetwork *nn,
    double         trainingInputs[nn->nbTrainingSets][nn->nbTraining]
                         [nn->nbInputNeurons],
    double trainingOutputs[nn->nbTraining][nn->nbOutputNeurons],
    size_t trainingIndexes[nn->nbTraining], const double learningRate,
    unsigned long nbEpochs)
{

  // Process to train the neural network
  for (size_t epoch = 0; epoch < nbEpochs; epoch++)
  {
    // Get a random training set index from the training sets indexes array
    size_t set = rand() % nn->nbTrainingSets; // for nbTrainingSets small = bad

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
        double acvn = 0.0; // nn->hiddenBiases[j];
        for (size_t k = 0; k < nn->nbInputNeurons; k++)
          acvn += trainingInputs[set][i][k] * nn->hiddenWeights[k][j];
        nn->hiddenLayer[j] = sigmoid(acvn);
      }

      // Output layer activation
      for (size_t j = 0; j < nn->nbOutputNeurons; j++)
      {
        double acvn = 0.0; // n->outputBiases[j];
        for (size_t k = 0; k < nn->nbHiddenNeurons; k++)
          acvn += nn->hiddenLayer[k] * nn->outputWeights[k][j];
        nn->outputLayer[j] = sigmoid(acvn);
      }

      // Print informations about current activation
      neuralNetworkPrintAssertOCR(nn, epoch, i + 1);

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

      /* ---- DESCENT ---- */
      // Apply the changes to the output layers weights
      for (size_t j = 0; j < nn->nbOutputNeurons; j++)
      {
        for (size_t k = 0; k < nn->nbHiddenNeurons; k++)
        {
          nn->outputWeights[k][j]
              += nn->hiddenLayer[k] * deltaOutput[j] * learningRate;
          nn->outputBiases[j]
              += deltaOutput[k] * learningRate * nn->hiddenLayer[k];
        }
      }

      // Apply the changes to the hidden layers weights
      for (size_t j = 0; j < nn->nbHiddenNeurons; j++)
      {
        for (size_t k = 0; k < nn->nbInputNeurons; k++)
        {
          nn->hiddenWeights[k][j]
              += trainingInputs[set][i][k] * deltaHidden[j] * learningRate;
          nn->hiddenBiases[j]
              += deltaHidden[k] * learningRate * trainingInputs[set][i][k];
        }
      }
    }
  }
}

void neuralNetworkPrintAssertOCR(NeuralNetwork *nn, unsigned long epoch,
                                 size_t expected)
{
  // Print the output layer
  printf("Epoch %lu: Output_Layer=[", epoch);
  for (size_t i = 0; i < nn->nbOutputNeurons - 1; i++)
    printf("%zu=%f,", i + 1, nn->outputLayer[i]);
  printf("9=%f] ", nn->outputLayer[nn->nbOutputNeurons - 1]);

  size_t interpretation
      = arrayMaxIndex(nn->outputLayer, nn->nbOutputNeurons) + 1;
  printf("Input=%zu, NN_Output=%zu ", expected, interpretation);

  nn->totalTries++;
  if (interpretation == expected)
  {
    printf("\033[0;32m");
    printf("OK\n");
    printf("\033[0m");
    nn->successCount++;
  }
  else
  {
    printf("\033[0;31m");
    printf("KO\n");
    printf("\033[0m");
  }
}

void neuralNetworkPrintResults(NeuralNetwork *nn, size_t maxEpochs)
{
  // printf("\n==== Hidden layers infos after %lu epochs ====\n", maxEpochs);
  // printf("Hidden layer weights:\t");
  // print2dArray(nn->hiddenWeights, nn->nbInputNeurons, nn->nbHiddenNeurons);
  // printf("Hidden layer biases:\t");
  // print1dArray(nn->hiddenBiases, nn->nbHiddenNeurons);

  // printf("\n==== Output layers infos after %lu epochs ====\n", maxEpochs);
  // printf("Output layer weights:\t");
  // print2dArray(nn->outputWeights, nn->nbHiddenNeurons, nn->nbOutputNeurons);
  // printf("Output layer biases:\t");
  // print1dArray(nn->outputBiases, nn->nbOutputNeurons);

  printf("Success rate for %zu epochs: %lf\n", maxEpochs,
         (nn->successCount / nn->totalTries) * 100);
}

void neuralNetworkSaveOCR(NeuralNetwork *nn, const char *filename)
{
  // Open the file
  FILE *file = fopen(filename, "w");

  // Write the layers sizes
  fprintf(file, "%lu %lu %lu %lu %lu\n", nn->nbInputNeurons, nn->nbHiddenNeurons,
          nn->nbOutputNeurons, nn->nbTraining, nn->nbTrainingSets);

  // Write the hidden layer weights
  for (size_t i = 0; i < nn->nbInputNeurons; i++)
  {
    for (size_t j = 0; j < nn->nbHiddenNeurons; j++)
      fprintf(file, "%lf\n", nn->hiddenWeights[i][j]);
  }

  // Write the hidden layer biases
  for (size_t i = 0; i < nn->nbHiddenNeurons; i++)
    fprintf(file, "%lf\n", nn->hiddenBiases[i]);

  // Write the output layer weights
  for (size_t i = 0; i < nn->nbHiddenNeurons; i++)
  {
    for (size_t j = 0; j < nn->nbOutputNeurons; j++)
      fprintf(file, "%lf\n", nn->outputWeights[i][j]);
  }

  // Write the output layer biases
  for (size_t i = 0; i < nn->nbOutputNeurons; i++)
    fprintf(file, "%lf\n", nn->outputBiases[i]);

  // Close the file
  fclose(file);
}
void neuralNetworkLoadOCR(NeuralNetwork *nn, const char *filename)
{
  // Open the file
  FILE *file = fopen(filename, "r");

  printf("\n===== LOADING PROCESS =====\nLoading OCR neural network from file "
         "%s...\n",
         filename);

  // Read the layers sizes
  fscanf(file, "%lu %lu %lu %lu %lu\n", &nn->nbInputNeurons, &nn->nbHiddenNeurons,
         &nn->nbOutputNeurons, &nn->nbTraining, &nn->nbTrainingSets);
  printf("Loading OCR neural network (inp=%lu, hid=%lu, out=%lu, train=%lu)\n",
         nn->nbInputNeurons, nn->nbHiddenNeurons, nn->nbOutputNeurons,
         nn->nbTraining);

  // Allocate the memory spaces for the layers
  printf("\nAllocating memory for the layers... ");
  nn->hiddenLayer = calloc(nn->nbHiddenNeurons, sizeof(double));
  if (nn->hiddenLayer == NULL)
    errx(1, "neuralNetworkLoadOCR: calloc failed for nn->hiddenLayer");

  nn->outputLayer = calloc(nn->nbOutputNeurons, sizeof(double));
  if (nn->outputLayer == NULL)
    errx(1, "neuralNetworkLoadOCR: calloc failed for nn->outputLayer");
  printf("DONE\n");

  printf("Allocating memory for the biases... ");
  nn->hiddenBiases = calloc(nn->nbHiddenNeurons, sizeof(double));
  if (nn->hiddenBiases == NULL)
    errx(1, "neuralNetworkLoadOCR: calloc failed for nn->hiddenBiases");

  nn->outputBiases = calloc(nn->nbOutputNeurons, sizeof(double));
  if (nn->outputBiases == NULL)
    errx(1, "neuralNetworkLoadOCR: calloc failed for nn->outputBiases");
  printf("DONE\n");

  // Allocate memory & load the hidden weights & biases arrays
  printf("\nAllocating memory for the hidden layer weights subarrays...\n");
  nn->hiddenWeights = calloc(nn->nbInputNeurons, sizeof(double *));
  if (nn->hiddenWeights == NULL)
    errx(1, "neuralNetworkLoadOCR: calloc failed for nn->hiddenWeights");

  printf("Loading hidden layer weights... ");
  for (size_t i = 0; i < nn->nbInputNeurons; i++)
  {
    nn->hiddenWeights[i]
        = (double *)calloc(nn->nbHiddenNeurons, sizeof(double));
    if (nn->hiddenWeights[i] == NULL)
      errx(1, "neuralNetworkLoadOCR: calloc failed for nn->hiddenWeights[%zu]",
           i);

    for (size_t j = 0; j < nn->nbHiddenNeurons; j++)
    {
      fscanf(file, "%lf\n", &nn->hiddenWeights[i][j]);
      // printf("Loading hidden weight [%lu, %lu] with value %lf\n", i, j,
      //  nn->hiddenWeights[i][j]);
    }
  }
  printf("DONE\n");

  printf("Loading hidden layer biases... ");
  for (size_t i = 0; i < nn->nbHiddenNeurons; i++)
  {
    fscanf(file, "%lf\n", &nn->hiddenBiases[i]);
    // printf("Loading hidden bias [%lu] with value %lf\n", i,
    //  nn->hiddenBiases[i]);
  }
  printf("DONE\n");

  // Allocate memory & load the hidden weights & biases arrays
  printf("\nAllocating memory for the output layer weights subarrays...\n");
  nn->outputWeights = calloc(nn->nbHiddenNeurons, sizeof(double *));
  if (nn->outputWeights == NULL)
    errx(1, "neuralNetworkLoadOCR: calloc failed for nn->outputWeights");

  printf("Loading output layer weights... ");
  for (size_t i = 0; i < nn->nbHiddenNeurons; i++)
  {
    nn->outputWeights[i]
        = (double *)calloc(nn->nbOutputNeurons, sizeof(double));
    if (nn->outputWeights[i] == NULL)
      errx(1, "neuralNetworkLoadOCR: calloc failed for nn->outputWeights[%zu]",
           i);

    for (size_t j = 0; j < nn->nbOutputNeurons; j++)
    {
      fscanf(file, "%lf\n", &nn->outputWeights[i][j]);
      // printf("Loading output weight [%lu, %lu] with value %lf\n", i, j,
      //  nn->outputWeights[i][j]);
    }
  }
  printf("DONE\n");

  printf("Loading output biases... ");
  for (size_t i = 0; i < nn->nbOutputNeurons; i++)
  {
    fscanf(file, "%lf\n", &nn->outputBiases[i]);
    // printf("Loading output bias [%lu] with value %lf\n", i,
    //        nn->outputBiases[i]);
  }
  printf("DONE\n");

  // Close the file
  fclose(file);
}

int neuralNetworkCompute(NeuralNetwork *nn, double *pixels)
{
  // Compute the hidden layer
  for (size_t j = 0; j < nn->nbHiddenNeurons; j++)
  {
    double acvn = 0.0; // nn->hiddenBiases[j];
    for (size_t k = 0; k < nn->nbInputNeurons; k++)
      acvn += pixels[k] * nn->hiddenWeights[k][j];
    nn->hiddenLayer[j] = sigmoid(acvn);
  }

  // Compute the output layer
  for (size_t j = 0; j < nn->nbOutputNeurons; j++)
  {
    double acvn = 0.0; // nn->outputBiases[j];
    for (size_t k = 0; k < nn->nbHiddenNeurons; k++)
      acvn += nn->hiddenLayer[k] * nn->outputWeights[k][j];
    nn->outputLayer[j] = sigmoid(acvn);
  }
  int max = arrayMaxIndex(nn->outputLayer, nn->nbOutputNeurons);
  return max + 1;
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