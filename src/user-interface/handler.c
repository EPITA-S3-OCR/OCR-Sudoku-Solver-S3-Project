#include "handler.h"

void onTrainButtonClicked(GtkButton *button, gpointer user_data)
{
  UserInterface *ui = (UserInterface *)user_data;
  // Get the number of epochs
  const char *epochs = gtk_entry_get_text(ui->nn->entryEpoch);
  printf("Epochs: %s\n", epochs);
  // Get the number of epochs in int
  unsigned long maxEpochs = strtoul(epochs, NULL, 10);

  // Get status of the verbose checkbox
  ocrNeuralNetworkUi(maxEpochs, ui, true);
}

void onImportButtonClicked(GtkButton *button, gpointer user_data)
{
  UserInterface *ui = (UserInterface *)user_data;

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
  }

  // Destroy the file picker
  gtk_widget_destroy(GTK_WIDGET(fileChooser));
}

void onSolveSudokuButtonClicked(GtkButton *button, gpointer user_data)
{
  UserInterface *ui = (UserInterface *)user_data;

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
    bool isHexa = gtk_toggle_button_get_active(ui->solver->hexaModeCheckbox);
    printf("Hexa mode: %s\n", isHexa ? "true" : "false");
    size_t size = 9 ? isHexa == false : 16;

    char sudoku[16][16];
    int  M[16][16] = {0};

    for (size_t i = 0; i < size; i++)
    {
      for (size_t j = 0; j < size; j++)
        M[i][j] = '0';
    }

    loadSudoku(sudoku, path, size);

    // Solve it bro

    system("rm src/user-interface/aver.png");
    system("rm src/user-interface/jeje.png");
    system("cp src/user-interface/sixbysix.png src/user-interface/aver.png");
    system("cp src/user-interface/defgrid.png src/user-interface/jeje.png");

    int   x        = (size == 16 ? 12 : 27);
    int   y        = (size == 16 ? 35 : 60);
    int   dis      = (size == 16 ? 38 : 67);
    int   fontsize = (size == 16 ? 40 : 60);
    char *original = (size == 16 ? "sixbysix.png" : "defgrid.png");
    char *modif    = (size == 16 ? "aver.png" : "jeje.png");

    char s[1000] = {0};

    for (size_t i = 0; i < size; i++)
    {
      for (size_t j = 0; j < size; j++)
      {
        if (sudoku[i][j] != '0')
        {
          sprintf(s, "convert -font unifont -pointsize %i -fill black \
                        -draw 'text %i,%i \"%c\"' \"src/user-interface/%s\" \
                        \"src/user-interface/%s\"",
                  fontsize, x, y, sudoku[i][j], modif, modif);
          system(s);
          M[i][j] = 1;
        }
        x += dis;
      }

      x = (size == 16 ? 12 : 27);
      y += dis;
    }

    Solve(sudoku, size);

    x = (size == 16 ? 12 : 27);
    y = (size == 16 ? 35 : 60);

    for (size_t i = 0; i < size; i++)
    {
      for (size_t j = 0; j < size; j++)
      {
        if (M[i][j] != 1)
        {
          sprintf(s, "convert -font unifont -pointsize %i -fill red \
                        -draw 'text %i,%i \"%c\"' \"src/user-interface/%s\" \
                        \"src/user-interface/%s\"",
                  fontsize, x, y, sudoku[i][j], modif, modif);
          system(s);
        }
        x += dis;
      }

      x = (size == 16 ? 12 : 27);
      y += dis;
    }
  }
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
  UserInterface *ui = (UserInterface *)user_data;

  // Rotate it by the angle
  int angle = gtk_range_get_value(GTK_RANGE(ui->ocr->rotateSlider));
  cairo_surface_t *image = rotate_surface(ui->sudokuLive, angle);

  // Save the cairo surface in "output/ui/current.jpg"
  // Convert the cairo surface to a pixbuf
  GdkPixbuf *pixbuf = gdk_pixbuf_get_from_surface(
      image, 0, 0, cairo_image_surface_get_width(image),
      cairo_image_surface_get_height(image));
  // Save the pixbuf as a jpeg file
  gdk_pixbuf_save(pixbuf, "output/ui/current.jpg", "jpeg", NULL, NULL);

  ui->verbose = gtk_toggle_button_get_active(
      GTK_TOGGLE_BUTTON(ui->ocr->verboseCheckbox));

  printf("Launching imageProcessingUi in a new thread");
  pthread_t thread;
  pthread_create(&thread, NULL, threadImageProcessing, ui);
}

void onWindowDestroy(GtkWidget *widget, gpointer user_data)
{
  g_print("Window destroyed\n");
  gtk_main_quit();
}
