#include "blur.h"

Uint32 get_average_color(SDL_Surface* surface,
                         int x,
                         int y,
                         int width,
                         int height,
                         int range)
{
  Uint32 sum_r = 0;
  Uint32 sum_g = 0;
  Uint32 sum_b = 0;
  Uint8 r, g, b;
  int count = 0;
  for (int i = -range; i <= range; i++)
    {
      for (int j = range; j <= range; j++)
        {
          if (x + i >= 0 && x + i < width && y + j >= 0 && y + j < height)
            {
              Uint32 color = get_pixel(surface, x + i, y + j);
              SDL_GetRGB(color, surface->format, &r, &g, &b);
              sum_r += r;
              sum_g += g;
              sum_b += b;
              count++;
            }
        }
    }

  return SDL_MapRGB(surface->format, sum_r / count, sum_g / count,
                    sum_b / count);
}

void blur(SDL_Surface* surface, int range)
{
  SDL_Surface* copy = copy_surface(surface);
  for (int i = 0; i < surface->w; i++)
    {
      for (int j = 0; j < surface->h; j++)
        {
          Uint32 color =
            get_average_color(copy, i, j, surface->w, surface->h, range);
          put_pixel(surface, i, j, color);
        }
    }
  SDL_FreeSurface(copy);
}