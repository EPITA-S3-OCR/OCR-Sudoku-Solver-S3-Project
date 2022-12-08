#include "handler.h"

void addConsoleMessage(UserInterface *ui, char *message)
{
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(ui->console);
  gtk_text_buffer_insert_at_cursor(buffer, message, -1);
}

void reloadImage(UserInterface *ui)
{
  // Load an image using Pixbuf
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(currentImagePath, NULL);
  pixbuf = gdk_pixbuf_scale_simple(pixbuf, 400, 400, GDK_INTERP_BILINEAR);
  // Set the image to the pixbuf
  gtk_image_set_from_pixbuf(ui->sudokuImage, pixbuf);
  // Free the pixbuf
  g_object_unref(pixbuf);
}

void copyImage(char *src, char *dst)
{
  FILE  *srcFile = fopen(src, "rb");
  FILE  *dstFile = fopen(dst, "wb");
  char   buffer[1024];
  size_t n;
  while ((n = fread(buffer, 1, sizeof buffer, srcFile)) > 0)
  {
    fwrite(buffer, 1, n, dstFile);
  }
  fclose(srcFile);
  fclose(dstFile);
}

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
    copyImage(path, currentImagePath);
    copyImage(path, savedImagePath);
    reloadImage(ui);
    free(path);
  }

  // Destroy the file picker
  gtk_widget_destroy(GTK_WIDGET(fileChooser));
}

void onRotateSliderChanged(GtkRange *range, gpointer user_data)
{
  UserInterface *ui    = (UserInterface *)user_data;
  int            angle = gtk_range_get_value(range);
  SDL_Surface   *image = IMG_Load(savedImagePath);
  image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGB888, 0);
  rotate(image, degreesToRadians(angle));
  IMG_SaveJPG(image, currentImagePath, 100);

  reloadImage(ui);
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
  bool verbose
      = gtk_toggle_button_get_active((GtkToggleButton *)ui->verboseCheckbox);
  g_print("Verbose: %s\n", verbose ? "true" : "false");
  imageProcessingUi(currentImagePath, verbose);
  // Save the image to "currentImagePath"
  // Reload the image from "currentImagePath" to the GtkImage
  reloadImage(ui);
  SDL_FreeSurface(image);
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
