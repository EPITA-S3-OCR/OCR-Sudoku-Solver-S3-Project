#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"
#include "image.h"
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

  double  *hiddenLayer;
  double  *outputLayer;
  double **hiddenWeights;
  double **outputWeights;
  double  *hiddenBiases;
  double  *outputBiases;
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

void neuralNetworkPrintAssertOCR(NeuralNetwork *nn, unsigned long epoch,
                                 size_t expected);
void neuralNetworkPrintResults(NeuralNetwork *nn, unsigned long maxEpochs);

void neuralNetworkSaveOCR(NeuralNetwork *nn, const char *filename);
void neuralNetworkLoadOCR(NeuralNetwork *nn, const char *filename);

int  neuralNetworkCompute(NeuralNetwork *nn, float *pixels);
void neuralNetworkFree(NeuralNetwork *nn);
#endif