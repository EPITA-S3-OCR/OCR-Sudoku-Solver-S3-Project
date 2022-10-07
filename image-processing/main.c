#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "blur.h"
#include "contrast.h"
#include "grayscale.h"
#include "hough_transform.h"
#include "sobel.h"
#include "utils.h"

int main(int argv, char** argc)
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
  SDL_Window* window =
    SDL_CreateWindow("image processing", SDL_WINDOWPOS_UNDEFINED,
                     SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  if (window == NULL)
    {
      errx(1, "SDL_CreateWindow: %s", SDL_GetError());
    }

  // Initialize renderer
  SDL_Renderer* renderer =
    SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL)
    {
      errx(1, "SDL_CreateRenderer: %s", SDL_GetError());
    }

  // Initialize surface by loading image in first argument
  SDL_Surface* surface = IMG_Load(argc[1]);
  if (surface == NULL)
    {
      errx(1, "IMG_Load: %s", SDL_GetError());
    }
  // Convert the format to be in 32bits every time to be able to use same functions later
  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);

  // Add effect(s) to surface
  contrast(surface);
  grayscale(surface);
  blur(surface, 1);
  // rotate(surface, 45);
  sobel_edge_detection(surface);
  hough_transform(surface);

  // Intialize a texture by converting the imported surface
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (texture == NULL)
    {
      errx(1, "SDL_CreateTextureFromSurface: %s", SDL_GetError());
    }

  SDL_Event event;
  int quit = 0;
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