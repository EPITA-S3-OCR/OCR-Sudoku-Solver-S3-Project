#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "err.h"
#include "utils.h"

// Blur the surface
void blur(SDL_Surface* surface, int range);