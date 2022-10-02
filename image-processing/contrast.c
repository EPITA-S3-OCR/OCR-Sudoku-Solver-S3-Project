#include "contrast.h"

// Contrast function (per pixel basis)
Uint8 f(Uint8 c, double n)
{
  if (c <= 255 / 2)
    return (Uint8)((255 / 2) * SDL_pow((double)2 * c / 255, n));
  else
    return 255 - f(255 - c, n);
}

void contrast(SDL_Surface* surface)
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
          r = f(r, 10);
          g = f(g, 10);
          b = f(b, 10);
          pixel = SDL_MapRGB(surface->format, r, g, b);
          put_pixel(surface, x, y, pixel);
        }
    }

  if (SDL_MUSTLOCK(surface))
    {
      SDL_UnlockSurface(surface);
    }
}
