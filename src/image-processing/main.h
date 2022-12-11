#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdbool.h>
#include "../user-interface/utils.h"
#include "detection/hough_transform.h"
#include "detection/local_threshold.h"
#include "detection/sobel.h"
#include "pretreatment/contrast.h"
#include "pretreatment/grayscale.h"
#include "pretreatment/noise_reduction.h"
#include "utils/lines.h"
#include "utils/linked_list.h"
#include "utils/splitting.h"
#include "utils/utils.h"

/**
 * @brief Does the image processing
 * @param verbose verbose or not
 *
 */
void imageProcessingUi(SDL_Surface *surface, bool verbose);

/**
 * @brief The main function for the rotation of an image
 * @param argv The number of arguments
 * @param argc The array of arguments
 * @return The exit code
 */
int imageRotateMain(int argv, char **argc);

/**
 * @brief The main function for the image processing
 * @param argc The number of arguments
 * @param argv The array of arguments
 *
 */
int imageProcessingMain(int argv, char **argc);
