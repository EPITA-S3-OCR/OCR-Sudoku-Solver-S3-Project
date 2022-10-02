#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Return the 32 bit value of a pixel from a surface at x, y coordinates
Uint32 get_pixel(SDL_Surface* surface, int x, int y);

// Set the value of a pixel into a surface at x,y coordinates
void put_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel);

// Return a SDL Surface of the same format
SDL_Surface* copy_surface(SDL_Surface* surface);