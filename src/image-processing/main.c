#include "main.h"

SDL_Surface *applyImageProcessing(SDL_Surface *surface, UserInterface *ui,
                                  bool verbose)
{
  printf("🧵Launching multi-thread image processing\n");
  if (verbose)
    addConsoleMessage(ui, "🧵Launching multi-thread image processing");
  // Apply the image processing
  // SDL_Surface *copy = surface; // copySurface(surface);
  SDL_Surface *copy = copySurface(surface);

  printf("📸 Applying grayscale...\n");
  if (verbose)
    addConsoleMessage(ui, "📸 Converting & applying to grayscale...");
  grayscale(copy);
  saveSurface(copy, "output/steps/1-grayscale.jpg");

  printf("🖌️ Applying contrast...\n");
  if (verbose)
    addConsoleMessage(ui, "🖌️ Applying contrast...");
  contrast(copy);
  saveSurface(copy, "output/steps/2-contrast.jpg");

  printf("🔊 Applying noise reduction...\n");
  if (verbose)
    addConsoleMessage(ui, "🔊 Applying noise detection & reduction...");
  denoise(copy);
  saveSurface(copy, "output/steps/3-denoise.jpg");

  printf("✨ Applying local threshold...\n");
  if (verbose)
    addConsoleMessage(ui, "✨ Applying local threshold...");
  localThreshold(copy);
  saveSurface(copy, "output/steps/4-local_threshold.jpg");

  printf("💽 Applying Sobel operator...\n");
  if (verbose)
    addConsoleMessage(ui, "💽 Applying Sobel operator...");
  sobelEdgeDetection(copy);
  saveSurface(copy, "output/steps/5-sobel.jpg");

  printf("📐Applying Hough transform algorithm...\n");
  if (verbose)
    addConsoleMessage(ui, "📐Applying Hough transform algorithm...");
  LinkedList *lines = malloc(sizeof(LinkedList));
  houghTransform(copy, lines);
  printf("        📈 Number of detected lines: %zu\n", lkLen(lines));
  if (verbose)
  {
    char *message = malloc(100);
    sprintf(message, "        📈 Number of detected lines: %zu", lkLen(lines));
    addConsoleMessage(ui, message);
    free(message);
  }
  SDL_Color color = {255, 0, 0, 255};
  printf("        🖍️ Drawing the lines...\n");
  if (verbose)
    addConsoleMessage(ui, "        🖍️ Drawing the lines...");
  SDL_Surface *surfacelines = copySurface(copy);
  drawLines(surfacelines, lines->next, color);
  saveSurface(surfacelines, "output/steps/6-lines.jpg");

  printf("📌 Getting the angle...\n");
  if (verbose)
    addConsoleMessage(ui, "📌 Getting the angle...");
  double angle = getRotationAngle(lines);

  printf("        ⭕ Found angle : %.2f\n", radiansToDegrees(angle));

  if (angle > 0.1 || angle < -0.1)
  {
    // printf("- Rotating the image...\n");
    // if (verbose)
    //   addConsoleMessage(ui, "Rotating the image...");
    rotate(copy, angle);
    rotateLines(lines, angle, copy->w, copy->h);
  }
  saveSurface(copy, "output/steps/8-rotate.jpg");

  LinkedList squares = squareDetection(lines, surface->w, surface->h);
  // size_t     nbSquares = lkLen(&squares);
  // LinkedList filteredSquares   = squareFilter(&squares);

  printf("📏 Drawing the squares...\n");
  if (verbose)
    addConsoleMessage(ui, "📏 Drawing the squares...");
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

  printf("️✂️  Splitting the image into tiles...\n");
  if (verbose)
    addConsoleMessage(ui, "✂️ Splitting the image into tiles...");
  printf("️💾  Resizing all the tiles (28x28)...\n");
  if (verbose)
    addConsoleMessage(ui, "💾 Resizing all the tiles (28x28)...");
  splitImage(copy, leftMostCell.xBottomLeft, leftMostCell.yBottomLeft, distX,
             distY);
  printf("✅ End of image processing\n");
  if (verbose)
    addConsoleMessage(ui, "✅ End of image processing");
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
  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);

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

void imageProcessingUi(char *filename, UserInterface *ui, bool verbose)
{
  SDL_Surface *surface = IMG_Load(filename);
  if (surface == NULL)
  {
    errx(1, "IMG_Load: %s", SDL_GetError());
  }
  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
  surface = applyImageProcessing(surface, ui, verbose);
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
  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);

  surface = applyImageProcessing(surface, NULL, false);
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