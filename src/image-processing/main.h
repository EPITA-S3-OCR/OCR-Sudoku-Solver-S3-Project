#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
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

int imageRotateMain(int argv, char **argc);
int imageProcessingMain(int argv, char **argc);