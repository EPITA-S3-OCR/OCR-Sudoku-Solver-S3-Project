#include "image.h"

void printPixels(double *pixels)
{
  for (int i = 0; i < 28 * 28; i++)
  {
    int pixel = pixels[i] * 255;
    printf("%3d", pixel);
    if (i % 28 == 0)
      printf("\n");
  }
}

double *loadImage(char *path)
{
  // Initialize the image
  SDL_Surface *surface = IMG_Load(path);
  if (surface == NULL)
    errx(1, "loadImage: Could not load image %s", path);

  // Convert the image to surface format
  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
  if (surface == NULL)
    errx(1, "loadImage: Could not convert image %s", path);

  // Allocate memory for the image & pixels array
  Uint32 *surfacePixels = (Uint32 *)surface->pixels;
  double *pixels        = malloc(surface->w * surface->h * sizeof(double));
  Uint8   r, g, b;

  // Load every pixel value in the pixels array
  for (int i = 0; i < surface->w * surface->h; i++)
  {
    // Get the current pixel & convert it to grayscale
    SDL_GetRGB(surfacePixels[i], surface->format, &r, &g, &b);
    pixels[i] = 255 ? r > 127 : 0;
  }

  // Free the memory allocated for the surface
  SDL_FreeSurface(surface);

  // Return the pixels array
  return pixels;
}