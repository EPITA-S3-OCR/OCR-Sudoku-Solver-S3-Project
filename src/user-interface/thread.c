#include "thread.h"

void generateFinalSudokuGrid(UserInterface *ui, char *path, int size)
{
  char sudoku[16][16];
  int  M[16][16] = {0};

  for (size_t i = 0; i < size; i++)
  {
    for (size_t j = 0; j < size; j++)
      M[i][j] = '0';
  }

  loadSudoku(sudoku, path, size);

  // Solve it bro
  MagickWandGenesis();
  MagickWand *magick_wand = NewMagickWand();
  MagickReadImage(magick_wand,
                  size == 9 ? "src/user-interface/generator/jeje.png"
                            : "src/user-interface/generator/sixbysix.png");
  DrawingWand *d_wand = NewDrawingWand();
  DrawSetFont(d_wand, "unifont");

  int x   = (size == 16 ? 12 : 27);
  int y   = (size == 16 ? 35 : 60);
  int dis = (size == 16 ? 38 : 67);
  // int   fontsize = (size == 16 ? 40 : 60);
  // int   fontsize = (size == 16 ? 32 : 60);
  // // char *original = (size == 16 ? "sixbysix.png" : "defgrid.png");
  // char *modif    = (size == 16 ? "aver.png" : "jeje.png");

  DrawSetFontSize(d_wand, size == 16 ? 32 : 50);

  char s[1000] = {0};

  for (size_t i = 0; i < size; i++)
  {
    for (size_t j = 0; j < size; j++)
    {
      if (sudoku[i][j] != '0')
      {
        char *thing = (char *)malloc(2 * sizeof(char));
        thing[0]    = sudoku[i][j];
        thing[1]    = '\0';
        DrawAnnotation(d_wand, x, y, thing);

        M[i][j] = 1;
      }
      x += dis;
      if (size == 9)
      {
        if (j == size / 2 - 1)
          x += 4;
      }
      else
      {
        if (j == size / 2 - 1)
          x += 4;
        else if (j == size / 4 - 1)
          x += 2;
      }
    }

    x = (size == 16 ? 12 : 27);
    y += dis;
    if (size == 9)
    {
      if (i == size / 2 - 1)
        y += 4;
    }
    else
    {
      if (i == size / 2 - 1)
        y += 4;
      else if (i == size / 4 - 1)
        y += 2;
    }
  }

  Solve(sudoku, size);

  // Set magick wand drawing color red
  PixelWand *color = NewPixelWand();
  PixelSetColor(color, "red");
  DrawSetFillColor(d_wand, color);

  x = (size == 16 ? 12 : 27);
  y = (size == 16 ? 35 : 60);

  for (size_t i = 0; i < size; i++)
  {
    for (size_t j = 0; j < size; j++)
    {
      if (M[i][j] != 1)
      {
        char *thing = (char *)malloc(2 * sizeof(char));
        thing[0]    = sudoku[i][j];
        thing[1]    = '\0';
        DrawAnnotation(d_wand, x, y, thing);
        // DrawAnnotation(d_wand, x, y, &(sudoku[i][j]));
      }
      x += dis;
      if (size == 9)
      {
        if (j == size / 2 - 1)
          x += 4;
      }
      else
      {
        if (j == size / 2 - 1)
          x += 4;
        else if (j == size / 4 - 1)
          x += 2;
      }
    }

    x = (size == 16 ? 12 : 27);
    y += dis;
    if (size == 9)
    {
      if (i == size / 2 - 1)
        y += 4;
    }
    else
    {
      if (i == size / 2 - 1)
        y += 4;
      else if (i == size / 4 - 1)
        y += 2;
    }
  }

  MagickDrawImage(magick_wand, d_wand);
  MagickWriteImage(magick_wand, "output/ui/output.png");
  magick_wand = DestroyMagickWand(magick_wand);
  MagickWandTerminus();
  loadImageUi(ui, "output/ui/output.png");
  displayImage(ui, ui->sudokuLive);
}

void *threadImageProcessing(void *data)
{
  printf("running image processing\n");
  UserInterface *ui = (UserInterface *)data;
  if (ui->sudokuLiveSDL == NULL)
  {
    printf("surface is null\n");
    return NULL;
  }
  imageProcessingUi(ui->sudokuLiveSDL, ui, ui->verbose);
  printf("image processing done\n");
  loadImageUi(ui, "output/ui/current.jpg");

  // Display the image
  displayImage(ui, ui->sudokuLive);

  // Reset slider to 0
  gtk_range_set_value(GTK_RANGE(ui->ocr->rotateSlider), 0);

  printf("🤖 Identifying cells content\n");
  if (ui->verbose)
    addConsoleMessage(ui, "🤖 Identifying cells content");
  ocrUi(ui, true);

  printf("🏁 Solving the sudoku\n");
  if (ui->verbose)
    addConsoleMessage(ui, "🏁 Solving the sudoku");
  generateFinalSudokuGrid(ui, "output/ui/sudoku", 9);
  printf("🏆 Sudoku solved\n");
  if (ui->verbose)
    addConsoleMessage(ui, "🏆 Sudoku solved");

  return NULL;
}
