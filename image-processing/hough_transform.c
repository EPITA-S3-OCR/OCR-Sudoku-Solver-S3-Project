#include "hough_transform.h"

#define THRESHOLD 0.6

void draw_line(SDL_Surface* surface,
               int xStart,
               int yStart,
               int xEnd,
               int yEnd,
               SDL_Color color)
{
  int x0 = xStart;
  int y0 = yStart;

  int x1 = xEnd;
  int y1 = yEnd;

  int dx = abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0);
  int sy = y0 < y1 ? 1 : -1;

  int err = dx + dy;

  int w = surface->w;
  int h = surface->h;
  while (1)
    {
      if (0 <= x0 && x0 < w && 0 <= y0 && y0 < h)
        {
          put_pixel(surface, x0, y0,
                    SDL_MapRGB(surface->format, color.r, color.g, color.b));
        }

      if (x0 == x1 && y0 == y1)
        break;

      int e2 = 2 * err;

      if (e2 >= dy)
        {
          err += dy;
          x0 += sx;
        }
      if (e2 <= dx)
        {
          err += dx;
          y0 += sy;
        }
    }
}

double degrees_to_radians(double degrees)
{
  return degrees * M_PI / 180.0;
}
double radians_to_degrees(double radian)
{
  return radian * 180.0 / M_PI;
}
unsigned int** initMatrice(unsigned int x, unsigned int y)
{
  unsigned int** matrice = NULL;
  matrice = calloc(y + 1, sizeof(unsigned int*));
  if (matrice == NULL)
    {
      errx(1, "Memory error");
    }
  for (size_t j = 0; j < y; j++)
    {
      matrice[j] = calloc(x + 1, sizeof(unsigned int));
      if (matrice[j] == NULL)
        {
          errx(1, "Memory error");
        }
    }
  return matrice;
}

void freeMatrice(unsigned int** matrice, size_t height)
{
  for (size_t y = 0; y < height; y++)
    {
      free(matrice[y]);
    }
  free(matrice);
}

void hough_transform(SDL_Surface* surface)
{
  // Save the image dimensions
  const double width = surface->w, height = surface->h;
  // Calculate the diagonal of the image
  const double diagonal = sqrt(width * width + height * height);

  // Initialize the constant values for theta and rho
  const double maxTheta = 180.0, minTheta = 0.0;
  const double maxRho = diagonal, minRho = -diagonal;
  const double nbRho = 2 * diagonal, nbTheta = nbRho;

  // Create an array of rhos
  double rhoStep = (maxRho - minRho) / nbRho;
  double* arrRhos = calloc(nbRho + 1, sizeof(double));
  int index = 0;
  for (double val = minRho; val <= maxRho && index < nbTheta;
       val += rhoStep, index++)
    {
      arrRhos[index] = val;
    }

  // Create an array of theta
  double step = (maxTheta - minTheta) / nbTheta;
  double* arrThetas = calloc(nbTheta + 1, sizeof(double));
  index = 0;
  for (double val = minTheta; val <= maxTheta && index < nbTheta;
       val += step, index++)
    {
      arrThetas[index] = val;
    }
  // Create a save of cos and sin value for each theta, to optimize
  // performance.
  double* saveCos = calloc(nbTheta + 1, sizeof(double));
  double* saveSin = calloc(nbTheta + 1, sizeof(double));
  for (int theta = 0; theta < nbTheta; theta++)
    {
      // Convert each value of theta's array into radians
      arrThetas[theta] = degrees_to_radians(arrThetas[theta]);

      // Save each cos(theta) and sin(theta) into their respective arrays
      saveCos[theta] = cos(arrThetas[theta]);
      saveSin[theta] = sin(arrThetas[theta]);
    }

  unsigned int** accumulator = initMatrice(nbTheta + 1, nbRho + 1);

  // We intialize the accumulator with all the value
  // In the same time, we search for the max value in the accumulator
  unsigned int max = 0;
  double rho;
  int croppedRho;

  for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
        {
          Uint32 pixel = get_pixel(surface, x, y);
          Uint8 r, g, b;
          SDL_GetRGB(pixel, surface->format, &r, &g, &b);
          if (r == 255)
            {
              for (int theta = 0; theta <= nbTheta; theta++)
                {
                  rho = x * saveCos[theta] + y * saveSin[theta];
                  croppedRho = rho + diagonal;
                  accumulator[croppedRho][theta]++;
                  if (accumulator[croppedRho][theta] > max)
                    {
                      max = accumulator[croppedRho][theta];
                    }
                }
            }
        }
    }

  free(saveCos);
  free(saveSin);

  double tempMaxTheta = 0.0;
  unsigned int histogram[181] = {0};
  unsigned int rounded_angle;

  int prev = accumulator[0][0];
  int prev_theta = 0, prev_rho = 0;
  int boolIsIncreasing = 1;
  int lineThreshold = max * THRESHOLD;
  SDL_Color pixel_color = {.r = 40, .g = 40, .b = 200};
  for (int theta = 0; theta <= nbTheta; theta++)
    {
      for (int rho = 0; rho <= nbRho; rho++)
        {
          int val = accumulator[rho][theta];

          if (val >= prev)
            {
              prev = val;
              prev_rho = rho;
              prev_theta = theta;
              boolIsIncreasing = 1;
              continue;
            }
          else if (val < prev && boolIsIncreasing)
            {
              boolIsIncreasing = 0;
            }
          else if (val < prev)
            {
              prev = val;
              prev_rho = rho;
              prev_theta = theta;
              continue;
            }

          if (val >= lineThreshold)
            {
              double r = arrRhos[prev_rho], t = arrThetas[prev_theta];

              if (t > tempMaxTheta)
                {
                  tempMaxTheta = t;
                  rounded_angle = (unsigned int)radians_to_degrees(t);
                  histogram[rounded_angle]++;
                }

              double c = cos(t), s = sin(t);
              // Calculate d0 point
              int d0X = (int)(c * r);
              int d0Y = (int)(s * r);
              // Calculate one point of the edge
              int xStart = d0X + (int)(diagonal * (-s));
              int yStart = d0Y + (int)(diagonal * c);

              // Calculate the other point of the edge
              int xEnd = d0X - (int)(diagonal * (-s));
              int yEnd = d0Y - (int)(diagonal * c);

              int theta = t;
              draw_line(surface, xStart, yStart, xEnd, yEnd, pixel_color);
            }
        }
    }

  // Free cos and sin arrays
  free(arrThetas);
  free(arrRhos);
  freeMatrice(accumulator, nbTheta + 1);
}