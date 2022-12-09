#include "handler.h"

void onImportButtonClicked(GtkButton *button, gpointer user_data)
{
  UserInterface *ui = (UserInterface *)user_data;
  addConsoleMessage(ui, "📸 1.1 Applying const");

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
    loadImageUi(ui, path);
  }

  // Destroy the file picker
  gtk_widget_destroy(GTK_WIDGET(fileChooser));
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
  g_print("Launch process button clicked\n");
  UserInterface *ui = (UserInterface *)user_data;
  // Load the image from "currentImagePath"
  SDL_Surface *image = IMG_Load(currentImagePath);
  if (image == NULL)
  {
    g_print("Failed to load image: %s\n", IMG_GetError());
    return;
  }
  image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGB888, 0);
  // Get the verbose checkbox state
  int  angle = gtk_range_get_value(ui->rotateSlider);
  bool verbose
      = gtk_toggle_button_get_active((GtkToggleButton *)ui->verboseCheckbox);
  g_print("Verbose: %s\n", verbose ? "true" : "false");
  imageProcessingUi(currentImagePath, verbose);
  // Save the image to "currentImagePath"
  loadImageUi(ui, currentImagePath);
  displayImage(ui, ui->sudokuLive);
}

void onVerboseCheckboxToggled(GtkToggleButton *togglebutton,
                              gpointer         user_data)
{
  g_print("Verbose checkbox toggled\n");
}

void onWindowDestroy(GtkWidget *widget, gpointer user_data)
{
  g_print("Window destroyed\n");
  gtk_main_quit();
}
