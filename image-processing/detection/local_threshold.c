#include "local_threshold.h"

// float noiseLevel(SDL_Surface* surface)
// {
//   const unsigned int width = surface->w;
//   const unsigned int height = surface->h;
//   float noiseThreshold = 0.5;
//   float count = 0.0;
//   double medium = 0.0;
//   for (unsigned int i = 0; i < width; i++)
//     {
//       for (unsigned int j = 0; j < height; j++)
//         {
//           medium = 0.0;
//           for (unsigned int k = 0; k < 9; k++)
//             {
//               Uint8* neighbours = getNeighbours(surface, i, j);
//               medium += neighbours[k];
//             }
//           medium /= 9;

//           // get pixel and its color
//           Uint32 pixel = getPixel(surface, i, j);
//           Uint8 r, g, b;
//           SDL_GetRGB(pixel, surface->format, &r, &g, &b);

//           double val = 1 - (.r / medium);
//           if (val < 0)
//             {
//               val *= -1;
//             }
//           if (val > noiseThreshold)
//             {
//               count++;
//             }
//         }
//     }

// return count;
// }

void localThreshold(SDL_Surface *surface)
{
  // Use the technique of local thresholding to binarize the image
  // The threshold is the average of the 9 pixels around the current pixel
  // If the current pixel is above the threshold, it is set to white
  // If the current pixel is below the threshold, it is set to black
  // The threshold is calculated for each pixel

  const double   threshold = 0.1; // or more when too much noise
  const int      width     = surface->w;
  const int      height    = surface->h;

  const int      s2        = fmax(width, height) / 16;
  unsigned long *image     = calloc(width * height, sizeof(unsigned long));
  if (image == NULL)
  {
    errx(1, "calloc");
  }
  long         sum   = 0;
  unsigned int count = 0;
  int          x1, y1, x2, y2;
  Uint32       color;
  Uint8        r, g, b;
  for (int y = 0; y < height; y++)
  {
    color = getPixel(surface, 0, y);
    SDL_GetRGB(color, surface->format, &r, &g, &b);
    sum += r;
    image[y] = sum;
  }

  for (int i = 1; i < width; i++)
  {
    sum = 0;
    for (int j = 0; j < height; j++)
    {
      color = getPixel(surface, i, j);
      SDL_GetRGB(color, surface->format, &r, &g, &b);
      sum += r;
      image[i * height + j] = image[(i - 1) * height + j] + sum;
    }
  }
  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < height; j++)
    {
      x1    = fmax(i - s2, 1);
      x2    = fmin(i + s2, width - 1);
      y1    = fmax(j - s2, 1);
      y2    = fmin(j + s2, height - 1);
      count = (x2 - x1) * (y2 - y1);
      sum   = image[x2 * height + y2] - image[x2 * height + (y1 - 1)]
            - image[(x1 - 1) * height + y2]
            + image[(x1 - 1) * height + (y1 - 1)];

      color = getPixel(surface, i, j);
      SDL_GetRGB(color, surface->format, &r, &g, &b);
      if (r * count < sum * (1.0 - threshold))
      {
        putPixel(surface, i, j, SDL_MapRGB(surface->format, 0, 0, 0));
      }
      else
      {
        putPixel(surface, i, j, SDL_MapRGB(surface->format, 255, 255, 255));
      }
    }
  }
  free(image);
}