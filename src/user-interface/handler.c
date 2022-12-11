#include "handler.h"

void duplicateFile(char *source, char *destination)
{
  FILE *sourceFile      = fopen(source, "rb");
  FILE *destinationFile = fopen(destination, "wb");

  char   buffer[1024];
  size_t bytesRead;

  while ((bytesRead = fread(buffer, 1, 1024, sourceFile)) > 0)
  {
    fwrite(buffer, 1, bytesRead, destinationFile);
  }

  fclose(sourceFile);
  fclose(destinationFile);
}

void onTrainButtonClicked(GtkButton *button, gpointer user_data)
{
  (void)button;
  UserInterface *ui = (UserInterface *)user_data;
  gtk_text_buffer_set_text(gtk_text_view_get_buffer(ui->console), "", -1);
  g_thread_new("threadTrain", threadTrain, ui);
}

void onImportButtonClicked(GtkButton *button, gpointer user_data)
{
  (void)button;
  UserInterface *ui = (UserInterface *)user_data;
  gtk_text_buffer_set_text(gtk_text_view_get_buffer(ui->console), "", -1);

  // Create a file picker for an image file
  GtkFileChooserNative *fileChooser = gtk_file_chooser_native_new(
      "Import Image", ui->window, GTK_FILE_CHOOSER_ACTION_OPEN, "_Open",
      "_Cancel");

  // Add filters so that only image files can be selected
  GtkFileFilter *filter = gtk_file_filter_new();
  gtk_file_filter_add_pixbuf_formats(filter);
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(fileChooser), filter);

  // Show the file picker
  gint response = gtk_native_dialog_run(GTK_NATIVE_DIALOG(fileChooser));

  // If the user selected a file, copy it to the current image path
  if (response == GTK_RESPONSE_ACCEPT)
  {
    char *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileChooser));
    g_print("Selected file: %s\n", path);
    // duplicate the image file under currentImagePath
    loadImageUi(ui, path);
    ui->sudokuLiveSDL = IMG_Load(path);
    // Set to RGB888
    ui->sudokuLiveSDL = SDL_ConvertSurfaceFormat(ui->sudokuLiveSDL,
                                                 SDL_PIXELFORMAT_RGB888, 0);
    // copy file to "output/ui/current.jpg"
  }
  // Reset the slide to 0
  gtk_range_set_value(GTK_RANGE(ui->ocr->rotateSlider), 0);
  // Destroy the file picker
  gtk_widget_destroy(GTK_WIDGET(fileChooser));
}

void onImportSolverButtonClicked(GtkButton *button, gpointer user_data)
{
  (void)button;
  UserInterface *ui = (UserInterface *)user_data;
  gtk_text_buffer_set_text(gtk_text_view_get_buffer(ui->console), "", -1);

  // Create a file picker for an image file
  GtkFileChooserNative *fileChooser = gtk_file_chooser_native_new(
      "Import Image", ui->window, GTK_FILE_CHOOSER_ACTION_OPEN, "_Open",
      "_Cancel");

  // Show the file picker
  gint response = gtk_native_dialog_run(GTK_NATIVE_DIALOG(fileChooser));

  // If the user selected a file, copy it to the current image path
  if (response == GTK_RESPONSE_ACCEPT)
  {
    char *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileChooser));
    g_print("Selected file: %s\n", path);
    // duplicate the image file under currentImagePath
    // Destroy the file picker
    gtk_widget_destroy(GTK_WIDGET(fileChooser));
    printf("Solving sudoku... %s\n", path);
    // get checkbox status
    bool isHexa = gtk_toggle_button_get_active(
        (GtkToggleButton *)ui->solver->hexadokuRadio);
    // bool isHexa =
    // gtk_toggle_button_get_active(ui->solver->hexaModeCheckbox);
    printf("Hexa mode: %s\n", isHexa ? "true" : "false");
    int size = isHexa == false ? 9 : 16;

    char sudoku[16][16];
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

    DrawSetFontSize(d_wand, size == 16 ? 32 : 50);

    for (int i = 0; i < size; i++)
    {
      for (int j = 0; j < size; j++)
      {
        if (sudoku[i][j] != '0')
        {

          unsigned char *digit = malloc(2 * sizeof(unsigned char));
          digit[0]             = sudoku[i][j];
          digit[1]             = '\0';
          DrawAnnotation(d_wand, x, y, digit);
          free(digit);
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
    ui->solver->sudokuPath = path;
    MagickDrawImage(magick_wand, d_wand);
    MagickWriteImage(magick_wand, "output/ui/output.png");
    magick_wand = DestroyMagickWand(magick_wand);
    MagickWandTerminus();
    loadImageUi(ui, "output/ui/output.png");
    displayImage(ui, ui->sudokuLive);
  }
}
void onSolveSudokuButtonClicked(GtkButton *button, gpointer user_data)
{
  (void)button;
  UserInterface *ui = (UserInterface *)user_data;
  gtk_text_buffer_set_text(gtk_text_view_get_buffer(ui->console), "", -1);
  char *path = ui->solver->sudokuPath;
  // get checkbox status
  bool isHexa = gtk_toggle_button_get_active(
      (GtkToggleButton *)ui->solver->hexadokuRadio);
  // bool isHexa =
  // gtk_toggle_button_get_active(ui->solver->hexaModeCheckbox);
  printf("Hexa mode: %s\n", isHexa ? "true" : "false");
  int size = isHexa == false ? 9 : 16;

  char sudoku[16][16];
  int  M[16][16] = {0};

  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
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

  DrawSetFontSize(d_wand, size == 16 ? 32 : 50);

  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      if (sudoku[i][j] != '0')
      {

        unsigned char *digit = malloc(2 * sizeof(unsigned char));
        digit[0]             = sudoku[i][j];
        digit[1]             = '\0';
        DrawAnnotation(d_wand, x, y, digit);
        free(digit);

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

  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      if (M[i][j] != 1)
      {
        unsigned char *digit = malloc(2 * sizeof(unsigned char));
        digit[0]             = sudoku[i][j];
        digit[1]             = '\0';
        DrawAnnotation(d_wand, x, y, digit);
        free(digit);
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

void onRotateSliderChanged(GtkRange *range, gpointer user_data)
{
  UserInterface *ui    = (UserInterface *)user_data;
  int            angle = gtk_range_get_value(range);

  cairo_surface_t *duplicated = duplicate_surface(ui->sudokuLive);
  duplicated                  = rotate_surface(duplicated, angle);

  displayImage(ui, duplicated);
}

void onLaunchProcessButtonClicked(GtkButton *button, gpointer user_data)
{
  (void)button;
  UserInterface *ui = (UserInterface *)user_data;
  gtk_text_buffer_set_text(gtk_text_view_get_buffer(ui->console), "", -1);

  // Rotate it by the angle
  // int angle = gtk_range_get_value(GTK_RANGE(ui->ocr->rotateSlider));
  // NULL);

  ui->verbose = gtk_toggle_button_get_active(
      GTK_TOGGLE_BUTTON(ui->ocr->verboseCheckbox));

  // printf("rotate image\n");
  // rotate(ui->sudokuLiveSDL, -degreesToRadians(angle));

  printf("Launching imageProcessingUi in a new thread\n");
  // pthread_create(&thread, NULL, threadImageProcessing, ui);
  g_thread_new("threadImageProcessing", threadImageProcessing, ui);
}

void onWindowDestroy()
{
  g_print("Window destroyed\n");
  gtk_main_quit();
}

void onHexadokuRadioToggled(GtkToggleButton *button, gpointer user_data)
{
  (void)button;
  UserInterface *ui = (UserInterface *)user_data;
  loadImageUi(ui, "src/user-interface/generator/sixbysix.png");
}

void onNormalSudokuRadioToggled(GtkToggleButton *button, gpointer user_data)
{
  (void)button;
  UserInterface *ui = (UserInterface *)user_data;

  loadImageUi(ui, "src/user-interface/generator/jeje.png");
}