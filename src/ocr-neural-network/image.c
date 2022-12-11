#include "image.h"

float *loadImage(char *path)
{
  SDL_Surface *surface = IMG_Load(path);
  if (surface == NULL)
    errx(1, "IMG_Load: %s", IMG_GetError());
  Uint32 *surfacePixels = (Uint32 *)surface->pixels;
  float  *pixels        = malloc(surface->w * surface->h * sizeof(float));
  Uint8   r, g, b;
  for (int i = 0; i < surface->w * surface->h; i++)
  {
    SDL_GetRGB(surfacePixels[i], surface->format, &r, &g, &b);
    pixels[i] = (float)r / 255.0;
  }
  return pixels;
}

void printPixels(float *pixels)
{
  for (int i = 0; i < 28 * 28; i++)
  {
    printf("%f\n", pixels[i]);
  }
}
