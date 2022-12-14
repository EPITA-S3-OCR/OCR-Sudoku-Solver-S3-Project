// Import librairies & helper files
#include <dirent.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../solver/solver.h"
#include "../user-interface/utils.h"
#include "helpers.h"
#include "image.h"
#include "neural_network.h"

#define INPUT_SIZE 784
#define HIDDEN_SIZE 16
#define OUTPUT_SIZE 9
#define TRAINING_SIZE 9
#define MAX_PATH_LENGTH 100
#define LEARNING_RATE 0.02

/// @brief Main function of the OCR neural network
/// @param argc The number of arguments
/// @param argv The arguments (string array)
/// @return The exit code
int ocrNeuralNetworkMain(int argc, char *argv[]);

/// @brief Main function to train OCR neural network used by the UI
/// @param epoch The number of epochs to train on
/// @param verbose If verbose or not.
/// @param ui The user interface
void ocrNeuralNetworkUi(int epoch, bool verbose);

/// @brief Calculates, using the NN, the numbers in the tiles and saves it in a
/// array.
void ocrUi();
