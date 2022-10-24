#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "detection/hough_transform.h"
#include "detection/local_threshold.h"
#include "detection/sobel.h"
#include "pretreatment/contrast.h"
#include "pretreatment/grayscale.h"
#include "pretreatment/noise_reduction.h"
#include "utils/lines.h"
#include "utils/linked_list.h"
#include "utils/utils.h"

void apply_image_processing(SDL_Surface *surface)
{
  // Apply the image processing
  SDL_Surface *copy = surface; // copySurface(surface);
  grayscale(copy);
  contrast(copy);
  denoise(copy);
  localThreshold(copy);
  sobelEdgeDetection(copy);

  LinkedList *lines = malloc(sizeof(LinkedList));
  houghTransform(copy, lines);
  // printf("lines: %zu\n", lkLen(lines));
  lineAverage(lines);
  // printf("lines: %zu\n", lkLen(lines));
  drawLines(surface, lines, (SDL_Color) {255, 0, 0, 255});
  highlight(surface, lines);
  double angle = getRotationAngle(lines);
  // printf("rotation angle: %f°\n", radiansToDegrees(angle));
  rotate(surface, angle);
  rotateLines(lines, angle, surface->w, surface->h);
  drawLines(surface, lines, (SDL_Color) {0, 0, 255, 255});
}

int main(int argv, char **argc)
{
  // Check arguments
  if (argv != 2)
  {
    errx(1, "usage: %s image", argc[0]);
  }

  // Initialize video
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    errx(1, "SDL_Init: %s", SDL_GetError());
  }

  // Initialize window
  SDL_Window *window
      = SDL_CreateWindow("image processing", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
  if (window == NULL)
  {
    errx(1, "SDL_CreateWindow: %s", SDL_GetError());
  }

  // Initialize renderer
  SDL_Renderer *renderer
      = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL)
  {
    errx(1, "SDL_CreateRenderer: %s", SDL_GetError());
  }

  // Initialize surface by loading image in first argument
  SDL_Surface *surface = IMG_Load(argc[1]);
  if (surface == NULL)
  {
    errx(1, "IMG_Load: %s", SDL_GetError());
  }

  // make the window ratio the same as the image with max h 1080
  int h = 800;
  int w = surface->w * h / surface->h;
  SDL_SetWindowSize(window, w, h);
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED);

  // Convert the format to be in 32bits every time to be able to use same
  // functions later
  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);

  apply_image_processing(surface);
  // Intialize a texture by converting the imported surface
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (texture == NULL)
  {
    errx(1, "SDL_CreateTextureFromSurface: %s", SDL_GetError());
  }

  SDL_Event event;
  int       quit = 0;
  // Event loop
  while (!quit)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        quit = 1;
      }
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  // Cleaning process
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}