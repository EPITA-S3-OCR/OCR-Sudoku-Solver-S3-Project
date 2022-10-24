// Import librairies & helper files
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "neural_network.h"
#include "math_nn.h"
#include "helpers.h"

#define INPUT_SIZE 2
#define HIDDEN_SIZE 2
#define OUTPUT_SIZE 1
#define TRAINING_SIZE 4

int main()
{
	// Generate random seed from current time
	srand(time(NULL));

	// Create & initialize a new neural network instance
	NeuralNetwork nn;
	neuralNetworkInit(&nn, INPUT_SIZE, HIDDEN_SIZE, OUTPUT_SIZE, TRAINING_SIZE);



	// Initialize the training inputs array
	double trainingInputs[TRAINING_SIZE][INPUT_SIZE] = {
		{0, 0},
		{0, 1},
		{1, 0},
		{1, 1}
	};

	// Initialize the training ouptuts array
	double trainingOutputs[TRAINING_SIZE][OUTPUT_SIZE] = {
		{0},
		{1},
		{1},
		{0}
	};

	const double learningRate = 0.1f;
	size_t trainingIndexes[TRAINING_SIZE] = {0, 1, 2, 3};
	unsigned long maxEpochs = 10000;

	// Process to train the neural network
	for (unsigned long epoch = 0; epoch < maxEpochs; epoch++)
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
		   	printf("Input: %g ^ %g\tOutput: %g\tPredicted output: %g\n",
				trainingInputs[i][0],
				trainingInputs[i][1],
				nn->outputLayer[0],
				trainingOutputs[i][0]
			);

		   	/* ---- BACKPROPAGATION ---- */
		   	// Change in output weights
		   	double deltaOutput[nn->nbOutputNeurons];
		   	for (size_t j = 0; j < nn->nbOutputNeurons; j++)
			{
				double error = trainingOutputs[i][j] - nn->outputLayer[j];
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
		   	for (size_t j = 0; j < nn->nbOutputNeurons; j++) {
			   	nn->outputBiases[j] += deltaOutput[j] * learningRate;

			   	for (size_t k = 0; k < nn->nbHiddenNeurons; k++)
					nn->outputWeights[k][j] +=
						nn->hiddenLayer[k] * deltaOutput[j] * learningRate;
			}

		   	// Apply the changes to the hidden layers weights
			for (size_t j = 0; j < nn->nbHiddenNeurons; j++) {
				nn->hiddenLayerBiases[j] += deltaHidden[j] * learningRate;

				for (size_t k = 0; k < nn->nbInputNeurons; k++)
					nn->hiddenWeights[k][j] +=
						trainingInputs[i][k] * deltaHidden[j] * learningRate;
		   	}
		}
	}

	// Print weights & biases values after 'maxEpochs' trains
	neuralNetworkPrintArrays(&nn, maxEpochs);
	return 0;
}
