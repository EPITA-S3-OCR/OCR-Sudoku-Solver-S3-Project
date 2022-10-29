#include "main.h"

void apply_image_processing(SDL_Surface *surface)
{
  // Apply the image processing
  // SDL_Surface *copy = surface; // copySurface(surface);
  SDL_Surface *copy = copySurface(surface);
  grayscale(copy);
  contrast(copy);
  denoise(copy);
  localThreshold(copy);
  sobelEdgeDetection(copy);

  LinkedList *lines = malloc(sizeof(LinkedList));
  houghTransform(copy, lines);
  // printf("lines: %zu\n", lkLen(lines));
  lineAverage(lines);
  printf("lines: %zu\n", lkLen(lines));
  // drawLines(surface, lines, (SDL_Color) {255, 0, 0, 255});
  // highlight(surface, lines);
  double angle = getRotationAngle(lines);
  // printf("rotation angle: %f°\n", radiansToDegrees(angle));
  // rotate(surface, angle);
  // rotateLines(lines, angle, surface->w, surface->h);
  // drawLines(surface, lines, (SDL_Color) {0, 0, 255, 255});

  copy = surface;
  grayscale(copy);
  contrast(copy);
  denoise(copy);
  localThreshold(copy);
  sobelEdgeDetection(copy);

  rotate(copy, angle);
  rotateLines(lines, angle, surface->w, surface->h);
  drawLines(copy, lines, (SDL_Color) {0, 0, 255, 255});

  LinkedList squares = squareDetection(lines, surface->w, surface->h);
  printf("%s\n", squares.next == NULL ? "null" : "not null");
  size_t nbSquares = lkLen(&squares);
  printf("squares: %zu\n", nbSquares);
  // LinkedList filteredSquares = squareFilter(&squares);
  // size_t     nbSquaresFiltered = lkLen(&filteredSquares);
  // printf("squares: %zu\n", nbSquaresFiltered);
  if (nbSquares > 0)
  {
    // drawSquares(copy, &squares, surface->w, surface->h);
  }

  LinkedList selectedSquare = {NULL, NULL, NULL};
  SudokuCell selectedCell   = selectLeftMostCell(&squares);
  printf("here\n");
  lkAppend(&selectedSquare, &selectedCell);
  printf("here5\n");
  drawSquares(copy, &selectedSquare, surface->w, surface->h);
  saveSurface(copy, "output.png");
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