#include "main.h"

SDL_Surface *applyImageProcessing(SDL_Surface *surface, UserInterface *ui,
                                  bool verbose)
{
  printf("Applying image processing...\n");
  saveSurface(surface, "output/steps/eazeazeazeaz-original.jpg");
  printf("- Copying surface...\n");
  SDL_Surface *copy = copySurface(surface);

  printf("- Applying grayscale...\n");
  grayscale(copy);
  saveSurface(copy, "output/steps/1-grayscale.jpg");

  printf("- Applying contrast...\n");
  contrast(copy);
  saveSurface(copy, "output/steps/2-contrast.jpg");

  printf("- Applying noise reduction...\n");
  denoise(copy);
  saveSurface(copy, "output/steps/3-denoise.jpg");
  saveSurface(copy, "output/steps/3-denoise-contrast.jpg");

  printf("- Applying local threshold...\n");
  localThreshold(copy);
  saveSurface(copy, "output/steps/4-local_threshold.jpg");

  SDL_Surface *surfaceToSplit = copySurface(copy);
  invert(surfaceToSplit);

  printf("- Applying sobel...\n");
  sobelEdgeDetection(copy);
  saveSurface(copy, "output/steps/5-sobel.jpg");

  printf("- Applying hough transform...\n");
  List *lines = initList();

  houghTransform(copy, lines);
  printf("  - lines: %d\n", listLength(lines));
  lineCleaning(lines);
  printf("here\n");
  printf("  - lines: %d\n", listLength(lines));

  SDL_Color black = {0, 0, 0, 255};
  drawLines(surfaceToSplit, lines, black, 15);

  SDL_Color    color        = {255, 0, 0, 255};
  SDL_Surface *surfacelines = copySurface(copy);
  drawLines(surfacelines, lines->next, color, 1);
  saveSurface(surfacelines, "output/steps/6-lines.jpg");

  printf("- Getting the angle...\n");
  double angle = getRotationAngle(lines);
  printf("  - Angle find : %f\n", radiansToDegrees(angle));

  if (angle > 0.1 || angle < -0.1)
  {
    printf("- Rotating the image...\n");
    rotate(copy, angle);
    rotate(surfaceToSplit, angle);
    lines = initList();
    houghTransform(copy, lines);
    lineCleaning(lines);
    // rotateLines(lines, angle, copy->w, copy->h);
  }
  saveSurface(copy, "output/steps/8-rotate.jpg");
  saveSurface(surfaceToSplit, "output/steps/draw-lines.jpg");

  List *squares = squareDetection(lines, surface->w, surface->h);
  printf("  - squares: %d\n", listLength(squares));

  squares = squareFilter(squares);
  printf("  - squares: %d\n", listLength(squares));

  printf("- Drawing the squares...\n");
  SDL_Surface *drawSelectedSquareSurface = copySurface(copy);
  SDL_Surface *drawSquareSurface         = copySurface(copy);
  drawSquares(drawSquareSurface, squares, color);
  saveSurface(drawSquareSurface, "output/steps/9-draw_squares.jpg");

  SudokuCell sudokuCell = selectSudoku(drawSelectedSquareSurface, squares);
  saveSurface(drawSelectedSquareSurface, "output/steps/10-draw_sudoku.jpg");

  int distX = sudokuCell.xTopRight - sudokuCell.xBottomLeft;
  int distY = sudokuCell.yTopRight - sudokuCell.yBottomLeft;

  splitImage(surfaceToSplit, sudokuCell.xBottomLeft, sudokuCell.yBottomLeft,
             distX, distY);
  return copy;
}

int imageRotateMain(int argv, char **argc)
{

  // Check arguments
  if (argv != 3)
  {
    errx(1, "usage: %s image angle", argc[0]);
  }

  // Initialize surface by loading image in first argument
  SDL_Surface *surface = IMG_Load(argc[1]);
  if (surface == NULL)
  {
    errx(1, "IMG_Load: %s", SDL_GetError());
  }
  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);

  // get the angle from the second argument
  double angle = degreesToRadians(atof(argc[2]));
  printf("rotation angle: %f° or %f rad\n", radiansToDegrees(angle), angle);

  rotate(surface, angle);

  SDL_FreeSurface(surface);

  return 0;
}

void imageProcessingUi(SDL_Surface *surface, UserInterface *ui, bool verbose)
{
  printf("imageProcessingUi\n");
  // SDL_Surface *surface = IMG_Load(filename);
  // if (surface == NULL)
  // {
  //   errx(1, "IMG_Load: %s", SDL_GetError());
  // }
  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
  printf("image converted\n");
  surface = applyImageProcessing(surface, ui, verbose);
  IMG_SaveJPG(surface, "output/ui/current.jpg", 100);
}

int imageProcessingMain(int argv, char **argc)
{
  // Check arguments
  if (argv != 2)
  {
    errx(1, "usage: %s image", argc[0]);
  }

  // Initialize surface by loading image in first argument
  SDL_Surface *surface = IMG_Load(argc[1]);
  if (surface == NULL)
  {
    errx(1, "IMG_Load: %s", SDL_GetError());
  }

  // Convert the format to be in 32bits every time to be able to use same
  // functions later
  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);

  surface = applyImageProcessing(surface, NULL, false);
  // Intialize a texture by converting the imported surface
  SDL_FreeSurface(surface);

  return 0;
}