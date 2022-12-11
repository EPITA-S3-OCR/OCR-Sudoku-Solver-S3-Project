#include "splitting.h"

int max(int a, int b)
{
  return a > b ? a : b;
}

SDL_Surface *resizeTo(SDL_Surface *surface, int w, int h)
{
  SDL_Surface *resized = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
  SDL_BlitScaled(surface, NULL, resized, NULL);
  return resized;
}

void splitImage(SDL_Surface *sfcSudoku, int orgX, int orgY, int w, int h)
{
  // Define the size of each sudoku tile
  int tileX = w / 9, tileY = h / 9;
  int sizeTile = max(tileX, tileY);

  // Loop through each sudoku tiles
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      // Create a new surface for the current tile
      SDL_Surface *sfcTile
          = SDL_CreateRGBSurface(0, sizeTile, sizeTile, 32, 0, 0, 0, 0);
      // set surface to mode RGBA_8888
      sfcTile = SDL_ConvertSurfaceFormat(sfcTile, SDL_PIXELFORMAT_RGBA8888, 0);
      // Create the rectangle at the correct coordinates
      SDL_Rect currTile;
      int      threshold = 5;

      currTile.x = j * sizeTile + orgX + threshold;
      currTile.y = i * sizeTile + orgY;
      currTile.w = sizeTile;
      currTile.h = sizeTile;

      // Copy the tile from the sudoku surface into the tile surface juste //
      // created
      SDL_BlitSurface(sfcSudoku, &currTile, sfcTile, NULL);

      // Save the tile to a new .BMP file
      char path[50]   = "output/tiles/";
      char filename[] = {i + '0', j + '0', '.', 'j', 'p', 'g', 0};
      strcat(path, filename);

      SDL_Surface *resizedSurface = resizeTo(sfcTile, 28, 28);
      resizedSurface              = SDL_ConvertSurfaceFormat(resizedSurface,
                                                             SDL_PIXELFORMAT_RGBA8888, 0);
      IMG_SaveJPG(resizedSurface, path, 100);

      // Free the surface created for the current tile
      SDL_FreeSurface(sfcTile);
    }
  }
}
