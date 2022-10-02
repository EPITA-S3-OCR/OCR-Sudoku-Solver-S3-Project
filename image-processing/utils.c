#include "utils.h"
#include <err.h>

void rotate(SDL_Surface* surface, int angle)
{
  SDL_Surface* copy = copy_surface(surface);
  double rad = angle * M_PI / 180;
  double cos_rad = cos(rad);
  double sin_rad = sin(rad);
  for (int i = 0; i < surface->w; i++)
    {
      for (int j = 0; j < surface->h; j++)
        {
          int x = (i - surface->w / 2) * cos_rad
            - (j - surface->h / 2) * sin_rad + surface->w / 2;
          int y = (i - surface->w / 2) * sin_rad
            + (j - surface->h / 2) * cos_rad + surface->h / 2;
          if (x >= 0 && x < surface->w && y >= 0 && y < surface->h)
            {
              Uint32 color = get_pixel(copy, x, y);
              put_pixel(surface, i, j, color);
            }
        }
    }
  SDL_FreeSurface(copy);
}

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
