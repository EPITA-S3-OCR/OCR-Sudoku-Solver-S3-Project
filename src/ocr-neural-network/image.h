#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

/// @brief Print the pixels array
/// @param pixels The given array populated with pixels grayscale values
void printPixels(double *pixels);

/// @brief Load the image from the given path
/// @param path The given path to the image to be loaded
/// @return The array containing pixels array
double *loadImage(char *path);