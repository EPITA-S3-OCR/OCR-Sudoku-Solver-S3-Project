#include "handler.h"

void reloadImage(userInterface *ui)
{
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(currentImagePath, NULL);
  pixbuf = gdk_pixbuf_scale_simple(pixbuf, 500, 500, GDK_INTERP_BILINEAR);
  gtk_image_set_from_pixbuf(ui->sudokuImage, pixbuf);
}

void openImage(char *filename, userInterface *ui)
{
  SDL_Surface *image = IMG_Load(filename);
  image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA8888, 0);
  IMG_SaveJPG(image, currentImagePath, 100);

  reloadImage(ui);
}

void rotateImage(int angle, userInterface *ui)
{
  SDL_Surface *image = IMG_Load(savedImagePath);
  image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA8888, 0);
  rotate(image, degreesToRadians(angle));
  IMG_SaveJPG(image, currentImagePath, 100);

  reloadImage(ui);
}

void onImportButtonClicked(GtkButton *button, gpointer user_data)
{
  userInterface *ui = (userInterface *)user_data;

  g_print("Import button clicked\n");
  // Create a new file chooser dialog for opening files
  GtkWidget *dialog = gtk_file_chooser_dialog_new(
      "Open File", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel",
      GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
  // Add filters, so that only certain file types can be selected
  GtkFileFilter *filter = gtk_file_filter_new();
  gtk_file_filter_add_pattern(filter, "*.png");
  gtk_file_filter_add_pattern(filter, "*.jpg");
  gtk_file_filter_add_pattern(filter, "*.jpeg");
  gtk_file_filter_set_name(filter, "Image files");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
  // Show the dialog and wait for a user response
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
  {
    // Get the filename from the dialog
    char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    // Open the image with the filename
    openImage(filename, ui);
    // Free the memory allocated by gtk_file_chooser_get_filename()
    g_free(filename);
  }
  // Destroy the dialog
  gtk_widget_destroy(dialog);
}

void onLaunchProcessButtonClicked(GtkButton *button, gpointer user_data)
{
  userInterface *ui = (userInterface *)user_data;
  g_print("Demo image button clicked\n");
  imageProcessingUi(currentImagePath);
  g_print("Finished processing\n");
  reloadImage(ui);
}

struct save
{
  int           *angle;
  userInterface *ui;
};

void onSliderChanged(GtkRange *range, gpointer user_data)
{
  struct save *s = (struct save *)user_data;

  *(s->angle) = gtk_range_get_value(range);
  g_print("Slider changed: %d\n", *(s->angle));
  rotateImage(*(s->angle), s->ui);
}
void finishedRotation(GtkWidget *widget, gpointer data)
{
  g_print("Destroy window\n");
  struct save *s     = (struct save *)data;
  SDL_Surface *image = IMG_Load("output/ui/current_saved.jpg");

  image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA8888, 0);
  g_print("Final angle: %d\n", *s->angle);
  rotate(image, degreesToRadians(*s->angle));
  IMG_SaveJPG(image, currentImagePath, 100);
  reloadImage(s->ui);
  SDL_FreeSurface(image);
}

void destroyWindow(GtkWidget *widget, gpointer data)
{
  gtk_widget_destroy(data);
}

void onManualRotationButtonClicked(GtkButton *button, gpointer user_data)
{
  g_print("Manual rotation button clicked\n");
  userInterface *ui = (userInterface *)user_data;

  SDL_Surface *image = IMG_Load(currentImagePath);
  image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA8888, 0);
  // save under a different name to not overwrite the original image
  IMG_SaveJPG(image, savedImagePath, 100);

  // Create a window with a slider and an exit button
  // react on value changed to rotate the image
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Manual rotation");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  GtkWidget *slider
      = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 360, 1);
  gtk_scale_set_draw_value(GTK_SCALE(slider), TRUE);
  gtk_range_set_value(GTK_RANGE(slider), 0);
  gtk_widget_set_hexpand(slider, TRUE);
  gtk_widget_set_vexpand(slider, TRUE);

  GtkWidget *exitButton = gtk_button_new_with_label("Exit");
  gtk_widget_set_hexpand(exitButton, TRUE);
  gtk_widget_set_vexpand(exitButton, TRUE);

  GtkWidget *grid = gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grid), slider, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), exitButton, 0, 1, 1, 1);

  gtk_container_add(GTK_CONTAINER(window), grid);

  gtk_widget_show_all(window);

  int angle = 0;

  struct save s = {
      .angle = &angle,
      .ui    = ui,
  };
  g_signal_connect(slider, "value-changed", G_CALLBACK(onSliderChanged), &s);
  // destroy window when exit button is clicked
  g_signal_connect(exitButton, "clicked", G_CALLBACK(finishedRotation), &s);
  g_signal_connect(exitButton, "clicked", G_CALLBACK(destroyWindow), window);
  // window);
  gtk_main();

  reloadImage(ui);
}

void onSudokuSizeButtonClicked()
{
  g_print("Sudoku size button clicked\n");
}

void onNeuralNetworkButtonClicked()
{
  g_print("Neural network button clicked\n");
}

void onDigitDetectionButtonClicked()
{
  g_print("Digit detection button clicked\n");
}

void onTrainNetworkButtonClicked()
{
  g_print("Train network button clicked\n");
}
