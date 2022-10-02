#include "grayscale.h"

void grayscale(SDL_Surface* surface)
{
  Uint8 r, g, b;
  Uint32 pixel;

  if (SDL_MUSTLOCK(surface))
    {
      if (SDL_LockSurface(surface) < 0)
        {
          warnx("can't lock surface");
        }
    }

  for (int y = 0; y < surface->h; y++)
    {
      for (int x = 0; x < surface->w; x++)
        {
          pixel = get_pixel(surface, x, y);
          SDL_GetRGB(pixel, surface->format, &r, &g, &b);
          Uint8 gray = (r + g + b) / 3;
          pixel = SDL_MapRGB(surface->format, gray, gray, gray);
          put_pixel(surface, x, y, pixel);
        }
    }

  if (SDL_MUSTLOCK(surface))
    {
      SDL_UnlockSurface(surface);
    }
}
