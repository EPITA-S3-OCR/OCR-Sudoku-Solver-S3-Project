#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

float *loadImage(char *path);
void   printPixels(float *pixels);