#include <err.h>
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
  size_t nbTrainingSets;

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
                       size_t nbTraining, size_t nbTrainingSets);

void neuralNetworkTrain(NeuralNetwork *nn, double ***trainingInputs,
                        double **trainingOutputs, size_t *trainingIndexes,
                        const double learningRate, unsigned long nbEpochs);

void neuralNetworkPrintAssertOCR(NeuralNetwork *nn, unsigned long epoch,
                                 size_t expected, size_t currentSet);
void neuralNetworkPrintResults(NeuralNetwork *nn, unsigned long maxEpochs);

void neuralNetworkSaveOCR(NeuralNetwork *nn, const char *filename);
void neuralNetworkLoadOCR(NeuralNetwork *nn, const char *filename);

int  neuralNetworkCompute(NeuralNetwork *nn, double *pixels);
void neuralNetworkFree(NeuralNetwork *nn);
#endif