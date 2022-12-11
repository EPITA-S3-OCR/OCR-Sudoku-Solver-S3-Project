#include "main.h"

SDL_Surface *applyImageProcessing(SDL_Surface *surface, UserInterface *ui,
                                  bool verbose)
{
  printf("🧵Launching multi-thread image processing\n");
  printf("----- \n");
  if (verbose)
  {
    g_idle_add(addConsoleMessage, "🧵Launching multi-thread image processing");
    g_idle_add(addConsoleMessage, "-----");
  }
  SDL_Surface *copy = copySurface(surface);

  printf("📸 Converting & applying to grayscale...\n");
  if (verbose)
  {
    g_idle_add(addConsoleMessage, "📸 Converting & applying to grayscale...");
  }

  grayscale(copy);
  saveSurface(copy, "output/steps/1-grayscale.jpg");

  printf("🖌️ Applying contrast...\n");
  if (verbose)
  {
    g_idle_add(addConsoleMessage, "🖌️ Applying contrast...");
  }
  contrast(copy);
  saveSurface(copy, "output/steps/2-contrast.jpg");

  printf("🔊 Applying noise detection & reduction...\n");
  if (verbose)
  {
    g_idle_add(addConsoleMessage, "🔊 Applying noise detection & reduction...");
  }
  denoise(copy);
  saveSurface(copy, "output/steps/3-denoise.jpg");
  saveSurface(copy, "output/steps/3-denoise-contrast.jpg");

  printf("✨ Applying local threshold...\n");
  if (verbose)
  {
    g_idle_add(addConsoleMessage, "✨ Applying local threshold...");
  }
  localThreshold(copy);
  saveSurface(copy, "output/steps/4-local_threshold.jpg");

  SDL_Surface *surfaceToSplit = copySurface(copy);
  invert(surfaceToSplit);

  printf("💽 Applying Sobel operator...\n");
  if (verbose)
  {
    g_idle_add(addConsoleMessage, "💽 Applying Sobel operator...");
  }
  sobelEdgeDetection(copy);
  saveSurface(copy, "output/steps/5-sobel.jpg");

  printf("📐Applying Hough transform algorithm...\n");
  if (verbose)
  {
    g_idle_add(addConsoleMessage, "📐Applying Hough transform algorithm...");
  }
  List *lines = initList();

  houghTransform(copy, lines);
  printf("        📈 Number of detected lines: %d\n", listLength(lines));
  if (verbose)
  {
    char *str = malloc(100);
    sprintf(str, "        📈 Number of cleaned lines: %d", listLength(lines));
    g_idle_add(addConsoleMessage, str);
    free(str);
  }
  lineCleaning(lines);
  printf("        📈 Number of cleaned lines: %d\n", listLength(lines));
  if (verbose)
  {
    char *str = malloc(100);
    sprintf(str, "        📈 Number of cleaned lines: %d", listLength(lines));
    g_idle_add(addConsoleMessage, str);
    free(str);
  }

  SDL_Color black = {0, 0, 0, 255};
  drawLines(surfaceToSplit, lines, black, 15);

  SDL_Color    color        = {255, 0, 0, 255};
  SDL_Surface *surfacelines = copySurface(copy);
  drawLines(surfacelines, lines->next, color, 1);
  saveSurface(surfacelines, "output/steps/6-lines.jpg");

  printf("📌 Getting the angle...\n");
  if (verbose)
  {
    g_idle_add(addConsoleMessage, "📌 Getting the angle...");
  }
  double angle = getRotationAngle(lines);
  printf("        ⭕ Found angle : %2f\n", radiansToDegrees(angle));
  if (verbose)
  {
    char *str = malloc(100);
    sprintf(str, "        ⭕ Found angle : %.2f", radiansToDegrees(angle));
    g_idle_add(addConsoleMessage, str);
    free(str);
  }

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

  printf("🟦 Applying square detection...\n");
  if (verbose)
  {
    g_idle_add(addConsoleMessage, "🟦 Applying square detection...");
  }
  List *squares = squareDetection(lines, surface->w, surface->h);
  printf("        📈 Number of detected lines: %d\n", listLength(squares));
  if (verbose)
  {
    char *str = malloc(100);
    sprintf(str, "        📈 Number of detected lines: %d",
            listLength(squares));
    g_idle_add(addConsoleMessage, str);
    free(str);
  }
  squares = squareFilter(squares);
  printf("        📈 Number of cleaned lines: %d\n", listLength(squares));
  if (verbose)
  {
    char *str = malloc(100);
    sprintf(str, "        📈 Number of cleaned lines: %d", listLength(squares));
    g_idle_add(addConsoleMessage, str);
    free(str);
  }
  // printf("- Drawing the squares...\n");
  SDL_Surface *drawSelectedSquareSurface = copySurface(copy);
  SDL_Surface *drawSquareSurface         = copySurface(copy);
  drawSquares(drawSquareSurface, squares, color);
  saveSurface(drawSquareSurface, "output/steps/9-draw_squares.jpg");

  SudokuCell sudokuCell = selectSudoku(drawSelectedSquareSurface, squares);
  saveSurface(drawSelectedSquareSurface, "output/steps/10-draw_sudoku.jpg");
  int distX = sudokuCell.xTopRight - sudokuCell.xBottomLeft;
  int distY = sudokuCell.yTopRight - sudokuCell.yBottomLeft;

  printf("✂️ Splitting the image into tiles...\n");
  printf("💾 Resizing all the tiles (28x28)...\n");
  if (verbose)
  {
    g_idle_add(addConsoleMessage, "✂️ Splitting the image into tiles...");
    g_idle_add(addConsoleMessage, "💾 Resizing all the tiles (28x28)...");
  }
  splitImage(surfaceToSplit, sudokuCell.xBottomLeft, sudokuCell.yBottomLeft,
             distX, distY);
  printf("🧵 Image processing finished\n");
  printf("----- \n");
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