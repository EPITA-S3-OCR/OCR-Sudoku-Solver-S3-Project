#include <stdio.h>
#include <stdlib.h>
#include "neural_network.h"
#include "helpers.h"

void neuralNetworkInit(NeuralNetwork *nn, size_t nbInputNeurons,
                       size_t nbHiddenNeurons, size_t nbOutputNeurons,
                       size_t nbTraining)
{
  // Initialize the layer properties of the neural network
  nn->nbInputNeurons  = nbInputNeurons;
  nn->nbHiddenNeurons = nbHiddenNeurons;
  nn->nbOutputNeurons = nbOutputNeurons;
  nn->nbTraining      = nbTraining;

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

void neuralNetworkPrintArrays(NeuralNetwork *nn, unsigned long maxEpochs)
{
  printf("==== Weights after %lu epochs ====\n", maxEpochs);
  print2dArray(nn->hiddenWeights, nn->nbInputNeurons, nn->nbHiddenNeurons);
  print2dArray(nn->outputWeights, nn->nbHiddenNeurons, nn->nbOutputNeurons);

  printf("==== Biases after %lu epochs ====\n", maxEpochs);
  print1dArray(nn->hiddenBiases, nn->nbHiddenNeurons);
  print1dArray(nn->outputBiases, nn->nbOutputNeurons);
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