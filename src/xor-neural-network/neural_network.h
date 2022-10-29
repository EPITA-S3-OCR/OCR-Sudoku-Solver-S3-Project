#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"
#include "math_nn.h"

#ifndef NEURAL_NETWORK_H
#  define NEURAL_NETWORK_H

typedef struct NeuralNetwork
{
  size_t nbInputNeurons;
  size_t nbHiddenNeurons;
  size_t nbOutputNeurons;
  size_t nbTraining;

  double successCount;
  double totalTries;

  double *hiddenLayer;
  double *outputLayer;
  double *hiddenWeights[2]; // find a way
  double *outputWeights[2]; // find a way
  double *hiddenBiases;
  double *outputBiases;
} NeuralNetwork;

void neuralNetworkInit(NeuralNetwork *nn, size_t nbInputNeurons,
                       size_t nbHiddenNeurons, size_t nbOutputNeurons,
                       size_t nbTraining);

void neuralNetworkTrain(
    NeuralNetwork *nn,
    double         trainingInputs[nn->nbTraining][nn->nbInputNeurons],
    double         trainingOutputs[nn->nbTraining][nn->nbOutputNeurons],
    size_t trainingIndexes[nn->nbTraining], const double learningRate,
    unsigned long nbEpochs);

void neuralNetworkPrintAssertXOR(NeuralNetwork *nn, unsigned long epoch, int a,
                                 int b);
void neuralNetworkPrintResults(NeuralNetwork *nn, unsigned long maxEpochs);

void neuralNetworkSaveXOR(NeuralNetwork *nn, const char *filename);
void neuralNetworkLoadXOR(NeuralNetwork *nn, const char *filename);

int neuralNetworkComputeMultiple(NeuralNetwork *nn, double *inputs);

double neuralNetworkCompute(NeuralNetwork *nn, double *inputs);

void neuralNetworkFree(NeuralNetwork *nn);
#endif