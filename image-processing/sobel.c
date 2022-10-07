#include "sobel.h"

Uint8 get_red(Uint32 color)
{
  return (color >> 24) & 0xFF;
}

int conv_x(int x, int y, SDL_Surface* surface)
{
  int sum = 0;
  sum += get_red(get_pixel(surface, x - 1, y - 1)) * -1;
  sum += get_red(get_pixel(surface, x - 1, y)) * -2;
  sum += get_red(get_pixel(surface, x - 1, y + 1)) * -1;
  sum += get_red(get_pixel(surface, x + 1, y - 1)) * 1;
  sum += get_red(get_pixel(surface, x + 1, y)) * 2;
  sum += get_red(get_pixel(surface, x + 1, y + 1)) * 1;
  return sum;
}

int conv_y(int x, int y, SDL_Surface* surface)
{
  int sum = 0;
  sum += get_red(get_pixel(surface, x - 1, y - 1)) * -1;
  sum += get_red(get_pixel(surface, x, y - 1)) * -2;
  sum += get_red(get_pixel(surface, x + 1, y - 1)) * -1;
  sum += get_red(get_pixel(surface, x - 1, y + 1)) * 1;
  sum += get_red(get_pixel(surface, x, y + 1)) * 2;
  sum += get_red(get_pixel(surface, x + 1, y + 1)) * 1;
  return sum;
}

void sobel_edge_detection(SDL_Surface* surface)
{
  SDL_Surface* copy = copy_surface(surface);
  for (int y = 1; y < surface->h - 1; y++)
    {
      for (int x = 1; x < surface->w - 1; x++)
        {
          int gx = conv_x(x, y, copy);
          int gy = conv_y(x, y, copy);
          int g = abs(gx) + abs(gy);

          Uint32 pixel = SDL_MapRGB(copy->format, g, g, g);
          put_pixel(surface, x, y,
                    (pixel > 0x00FFFFFF) ? 0xFFFFFFFF : 0x00000000);
        }
    }

  SDL_FreeSurface(copy);
}