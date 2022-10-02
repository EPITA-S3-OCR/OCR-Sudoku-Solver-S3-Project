#include <err.h>

#include "utils.h"
Uint32 get_pixel(SDL_Surface* surface, int x, int y)
{
  Uint32* pixels = (Uint32*)surface->pixels;
  return pixels[y * surface->w + x];
}

void put_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
  // Our pixels are on 32bits in SDL_PIXELFORMAT_RGBA8888
  Uint32* pixels = surface->pixels;
  pixels[y * surface->w + x] = pixel;
}

SDL_Surface* copy_surface(SDL_Surface* surface)
{
  Uint32* newPixels = malloc((size_t)surface->pitch * surface->h);
  if (newPixels == NULL)
    {
      errx(1, "malloc");
    }
  memcpy(newPixels, surface->pixels, (size_t)surface->pitch * surface->h);
  SDL_Surface* copy =
    SDL_CreateRGBSurfaceWithFormatFrom(newPixels, surface->w, surface->h, 32,
                                       surface->pitch, surface->format->format);
  return copy;
}
