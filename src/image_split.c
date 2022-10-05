#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void splitImage(SDL_Surface* sfcSudoku, int orgX, int orgY, int w, int h)
{
	// Define the size of each sudoku tile
	int tileX = w/9, tileY = h/9;

	// Loop through each sudoku tiles
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			// Create a new surface for the current tile
			SDL_Surface* sfcTile = SDL_CreateRGBSurface(0, tileX, tileY, 32, 0, 0, 0, 0);

			// Create the rectangle at the correct coordinates
			SDL_Rect currTile;
			int threshold = 5;
			currTile.x = j*tileX + orgX + threshold;
			currTile.y = i*tileY + orgY;
			currTile.w = tileX;
			currTile.h = tileY;

			// Copy the tile from the sudoku surface into the tile surface juste created 
			SDL_BlitSurface(sfcSudoku, &currTile, sfcTile, NULL);

			// Save the tile to a new .BMP file
			char path[50] = "Tiles/";
			char filename[3] = {i+'0', j+'0', 0};
			strcat(path, filename);
			SDL_SaveBMP(sfcTile, path);

			// Free the surface created for the current tile
			SDL_FreeSurface(sfcTile);
		}
	}
}

int main(int argc, char** argv)
{
	// Checks the number of arguments.
	if (argc != 2)
		errx(EXIT_FAILURE, "Usage: image-file");

	// Create a surface to split from
	SDL_Surface* surface = IMG_Load(argv[1]);
	if (surface == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	// Split the image into 81 tiles to feed to the AI
	int ORGX = 0, ORGY = 0, W = 1000, H = 1000;		// sudoku1
	//int ORGX = 342, ORGY = 216, W = 830, H = 821;		// sudoku2
	//int ORGX = 130, ORGY = 89, W = 520, H = 520;		// sudoku3
	//int ORGX = 415, ORGY = 162, W = 1132, H = 1147;	// sudoku4
	splitImage(surface, ORGX, ORGY, W, H);

	// Free the main surface
	SDL_FreeSurface(surface);

	return EXIT_SUCCESS;
}
