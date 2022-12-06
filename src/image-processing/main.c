#include "main.h"

SDL_Surface *applyImageProcessing(SDL_Surface *surface)
{
  printf("Applying image processing...\n");
  // Apply the image processing
  // SDL_Surface *copy = surface; // copySurface(surface);
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

  printf("- Applying local threshold...\n");
  localThreshold(copy);
  saveSurface(copy, "output/steps/4-local_threshold.jpg");

  printf("- Applying sobel...\n");
  sobelEdgeDetection(copy);
  saveSurface(copy, "output/steps/5-sobel.jpg");

  printf("- Applying hough transform...\n");
  LinkedList *lines = malloc(sizeof(LinkedList));
  houghTransform(copy, lines);
  printf("  - lines: %zu\n", lkLen(lines));

  SDL_Color color = {255, 0, 0, 255};

  SDL_Surface *surfacelines = copySurface(copy);
  drawLines(surfacelines, lines->next, color);
  saveSurface(surfacelines, "output/steps/6-lines.jpg");

  printf("- Getting the angle...\n");
  double angle = getRotationAngle(lines);
  printf("  - Angle find : %f\n", radiansToDegrees(angle));

  if (angle > 0.1 || angle < -0.1)
  {
    printf("- Rotating the image...\n");
    rotate(copy, angle);
    rotateLines(lines, angle, copy->w, copy->h);
  }
  saveSurface(copy, "output/steps/8-rotate.jpg");

  LinkedList squares = squareDetection(lines, surface->w, surface->h);
  // size_t     nbSquares = lkLen(&squares);
  // LinkedList filteredSquares   = squareFilter(&squares);

  printf("- Drawing the squares...\n");
  SDL_Surface *drawSquareSurface = copySurface(copy);
  drawSquares(drawSquareSurface, &squares, color);
  saveSurface(drawSquareSurface, "output/steps/9-draw_squares.jpg");

  // set the upper left pixel to red
  SudokuCell leftMostCell  = selectLeftMostCell(&squares);
  SudokuCell rightMostCell = selectRightMostCell(&squares);
  saveSurface(copy, "output/output.png");

  // new linkedlist
  // LinkedList  start       = {NULL, NULL, 0};
  // LinkedList *mainSquares = &start;
  // mainSquares             = lkAppend(mainSquares, &leftMostCell);
  // mainSquares             = lkAppend(mainSquares, &rightMostCell);

  // drawSquares(copy, &start, surface->w, surface->h, color);

  drawLine(copy, leftMostCell.xBottomLeft, leftMostCell.yBottomLeft,
           rightMostCell.xTopRight, rightMostCell.yTopRight, color);

  saveSurface(copy, "output/output2.png");
  int distX = rightMostCell.xTopRight - leftMostCell.xBottomLeft;
  int distY = rightMostCell.xTopRight - leftMostCell.xBottomLeft;

  printf("- Splitting the image...\n");
  splitImage(copy, leftMostCell.xBottomLeft, leftMostCell.yBottomLeft, distX,
             distY);
  printf("End of image processing\n");
  return copy;
}
int imageRotateMain(int argv, char **argc)
{
  // Check arguments
  if (argv != 3)
  {
    errx(1, "usage: %s image angle", argc[0]);
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
  int h = 900;
  int w = surface->w * h / surface->h;
  SDL_SetWindowSize(window, w, h);
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED);

  // Convert the format to be in 32bits every time to be able to use same
  // functions later
  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);

  // get the angle from the second argument
  double angle = degreesToRadians(atof(argc[2]));
  printf("rotation angle: %f° or %f rad\n", radiansToDegrees(angle), angle);

  rotate(surface, angle);

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

void imageProcessingUi(char *filename)
{
  SDL_Surface *surface = IMG_Load(filename);
  if (surface == NULL)
  {
    errx(1, "IMG_Load: %s", SDL_GetError());
  }
  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
  surface = applyImageProcessing(surface);
  IMG_SaveJPG(surface, filename, 100);
}

int imageProcessingMain(int argv, char **argc)
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
  int h = 900;
  int w = surface->w * h / surface->h;
  SDL_SetWindowSize(window, w, h);
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED);

  // Convert the format to be in 32bits every time to be able to use same
  // functions later
  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);

  surface = applyImageProcessing(surface);
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