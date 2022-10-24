#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H
#include <stdlib.h>

struct NeuralNetwork
{
    size_t nbInputNeurons;
    size_t nbHiddenNeurons;
    size_t nbOutputNeurons;
    size_t nbTraining;

    double *hiddenLayer;
    double *outputLayer;
    double *hiddenWeights[2]; // find a way
    double *outputWeights[2]; // find a way
    double *hiddenBiases;
    double *outputBiases;
} typedef NeuralNetwork;

void neuralNetworkInit(
    NeuralNetwork *nn,
    size_t nbInputNeurons,
    size_t nbHiddenNeurons,
    size_t nbOutputNeurons,
    size_t nbTraining
);

void neuralNetworkPrintArrays(NeuralNetwork *nn, unsigned long maxEpochs);
void neuralNetworkFree(NeuralNetwork *nn);

#endif