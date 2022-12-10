// Import librairies & helper files
#include <dirent.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "helpers.h"
#include "image.h"
#include "neural_network.h"

#define INPUT_SIZE 784
#define HIDDEN_SIZE 16
#define OUTPUT_SIZE 9
#define TRAINING_SIZE 9
#define MAX_PATH_LENGTH 100
// #define LEARNING_RATE 0.2
#define LEARNING_RATE 0.02
// #define LEARNING_RATE 0.8

size_t countFolders(char *folderRoot);
int    ocrNeuralNetworkMain(int argc, char *argv[]);