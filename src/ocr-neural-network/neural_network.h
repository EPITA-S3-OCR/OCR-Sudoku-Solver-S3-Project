#pragma once

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../user-interface/main.h"
#include "../user-interface/utils.h"
#include "helpers.h"
#include "image.h"
#include "math_nn.h"

/**
 * @brief The Neural Network structure
 *
 */
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

/// @brief Forward propagate the given input through the neural network
/// @param nn The neural network
/// @param input The input to forward propagate
void forwardPropagation(NeuralNetwork *nn, double *input);

/// @brief Backpropagation algorithm for training a neural network
/// @param nn The neural network to train
/// @param expectedOutput The expected output of the neural network
/// @param deltaOutput The delta of the output layer
/// @param deltaHidden The delta of the hidden layer
void backPropagation(NeuralNetwork *nn, double *expectedOutput,
                     double deltaOutput[nn->nbOutputNeurons],
                     double deltaHidden[nn->nbHiddenNeurons]);

/// @brief Stochastic gradient descent
/// @param nn The neural network
/// @param input The input to forward propagate
/// @param deltaOutput The delta of the output layer
/// @param deltaHidden The delta of the hidden layer
/// @param learningRate The learning rate
void descent(NeuralNetwork *nn, double *input,
             double deltaOutput[nn->nbOutputNeurons],
             double deltaHidden[nn->nbHiddenNeurons], double learningRate);

/// @brief Initializes a neural network & allocates memory for it.
/// @param nn The neural network to create.
/// @param nbInputNeurons The number of input neurons.
/// @param nbHiddenNeurons The number of hidden neurons.
/// @param nbOutputNeurons The number of output neurons.
/// @param nbTraining The number of training iterations.
/// @param nbTrainingSets The number of training sets.
void neuralNetworkInit(NeuralNetwork *nn, size_t nbInputNeurons,
                       size_t nbHiddenNeurons, size_t nbOutputNeurons,
                       size_t nbTraining, size_t nbTrainingSets);

/// @brief Trains a neural network with a set of training data.
/// @param nn The neural network to train.
/// @param trainingInputs The training inputs.
/// @param trainingOutputs The training outputs.
/// @param trainingIndexes The training indexes.
/// @param learningRate The learning rate.
/// @param nbEpochs The number of epochs.
void neuralNetworkTrain(NeuralNetwork *nn, double ***trainingInputs,
                        double **trainingOutputs, size_t *trainingIndexes,
                        const double learningRate, unsigned long nbEpochs,
                        bool verbose);

/// @brief Asserts the accuracy of a neural network.
/// @param nn The neural network to assert.
/// @param epoch The current epoch.
/// @param expected The expected output.
void neuralNetworkPrintAssertOCR(NeuralNetwork *nn, unsigned long epoch,
                                 size_t expected);

/// @brief Prints the accuracy of a neural network.
/// @param nn The neural network to print.
/// @param maxEpochs The maximum number of epochs.
void neuralNetworkPrintResults(NeuralNetwork *nn, unsigned long maxEpochs);

/// @brief Saves the weights & biases of a neural network.
/// @param nn The neural network to save.
/// @param filename The filename to save to.
void neuralNetworkSaveOCR(NeuralNetwork *nn, const char *filename);

/// @brief Loads the weights & biases of a neural network.
/// @param nn The neural network to load.
/// @param filename The filename to load from.
void neuralNetworkLoadOCR(NeuralNetwork *nn, const char *filename);

/// @brief Computes the output of a neural network.
/// @param nn The neural network to compute the output of.
/// @param pixels The pixels of the image to compute the output of.
/// @return The output of the neural network.
int neuralNetworkCompute(NeuralNetwork *nn, double *pixels);

/// @brief Frees a neural network.
/// @param nn The neural network to free.
void neuralNetworkFree(NeuralNetwork *nn);