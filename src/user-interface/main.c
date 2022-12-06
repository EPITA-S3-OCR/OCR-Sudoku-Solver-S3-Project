#include "main.h"

void loadCSS()
{
  // add css provider from the file "mystyle.css"
  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(provider, "src/user-interface/mystyle.css",
                                  NULL);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void useImage(userInterface *ui)
{
  g_print("Is Image ? %s",
          GTK_IS_IMAGE(&(ui->sudokuImage)) ? "true" : "false");
}

void onResize(GtkWindow *window, gpointer user_data)
{
  g_print("Window resized");
  userInterface *ui = (userInterface *)user_data;
  useImage(ui);
  // Resize the size of the pixbuf in the image proportionally to the
  // window size
  GdkPixbuf *pixbuf = gtk_image_get_pixbuf(ui->sudokuImage);

  if (pixbuf != NULL)
  {
    int width  = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    g_print("Image size: %d x %d\n", width, height);

    int windowWidth, windowHeight;
    gtk_window_get_size(window, &windowWidth, &windowHeight);
    g_print("Window size: %d x %d\n", windowWidth, windowHeight);

    // Calculate the new size of the pixbuf
    int newWidth  = windowWidth - 100;
    int newHeight = windowHeight - 100;
    if (width > height)
    {
      newHeight = (int)((float)height / (float)width * (float)newWidth);
    }
    else
    {
      newWidth = (int)((float)width / (float)height * (float)newHeight);
    }

    g_print("New size: %d x %d\n", newWidth, newHeight);

    // Scale the pixbuf
    GdkPixbuf *scaledPixbuf = gdk_pixbuf_scale_simple(
        pixbuf, newWidth, newHeight, GDK_INTERP_BILINEAR);

    // Set the new pixbuf
    gtk_image_set_from_pixbuf(ui->sudokuImage, scaledPixbuf);

    // Free the pixbuf
    g_object_unref(scaledPixbuf);
  }
}

int uiMain(int argc, char *argv[])
{
  // Initializes GTK.
  gtk_init(NULL, NULL);

  // Load CSS
  loadCSS();

  // Loads the UI description and builds the UI.
  // (Exits if an error occurs.)
  GtkBuilder *builder = gtk_builder_new();
  GError     *error   = NULL;
  if (gtk_builder_add_from_file(builder, "src/user-interface/ui.glade", &error)
      == 0)
  {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return 1;
  }

  // Gets the widgets.
  GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));

  // Gets the button with the id "ImportButton"
  GtkButton *importButton
      = GTK_BUTTON(gtk_builder_get_object(builder, "ImportButton"));
  GtkButton *launchProcessButton
      = GTK_BUTTON(gtk_builder_get_object(builder, "LaunchProcess"));
  GtkButton *manualRotationButton
      = GTK_BUTTON(gtk_builder_get_object(builder, "ManualRotation"));
  GtkButton *sudokuSizeButton
      = GTK_BUTTON(gtk_builder_get_object(builder, "SudokuSize"));
  GtkButton *digitDetectionButton
      = GTK_BUTTON(gtk_builder_get_object(builder, "DigitDetection"));
  GtkButton *trainNetworkButton
      = GTK_BUTTON(gtk_builder_get_object(builder, "TrainNetwork"));
  GtkImage *sudokuImage
      = GTK_IMAGE(gtk_builder_get_object(builder, "SudokuImage"));

  // Assigns the ui structure
  userInterface ui = {
      .window               = window,
      .importButton         = importButton,
      .launchProcessButton  = launchProcessButton,
      .manualRotationButton = manualRotationButton,
      .sudokuSizeButton     = sudokuSizeButton,
      .digitDetectionButton = digitDetectionButton,
      .trainNetworkButton   = trainNetworkButton,
      .sudokuImage          = sudokuImage,
  };

  // Connects signal handlers.
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  // g_signal_connect(window, "configure-event", G_CALLBACK(onResize), &ui);

  g_signal_connect(importButton, "clicked", G_CALLBACK(onImportButtonClicked),
                   &ui);
  g_signal_connect(launchProcessButton, "clicked",
                   G_CALLBACK(onLaunchProcessButtonClicked), &ui);
  g_signal_connect(manualRotationButton, "clicked",
                   G_CALLBACK(onManualRotationButtonClicked), &ui);
  g_signal_connect(sudokuSizeButton, "clicked",
                   G_CALLBACK(onSudokuSizeButtonClicked), &ui);
  g_signal_connect(digitDetectionButton, "clicked",
                   G_CALLBACK(onDigitDetectionButtonClicked), &ui);
  g_signal_connect(trainNetworkButton, "clicked",
                   G_CALLBACK(onTrainNetworkButtonClicked), &ui);

  // Set an image of 100x100 pixels
  GdkPixbuf *pixbuf
      = gdk_pixbuf_new_from_file("src/user-interface/sudoku.jpg", NULL);
  // Resize to 100x100
  pixbuf = gdk_pixbuf_scale_simple(pixbuf, 400, 400, GDK_INTERP_BILINEAR);

  gtk_image_set_from_pixbuf(sudokuImage, pixbuf);
  // set it's position to the vertical center of the window
  gtk_widget_set_halign(GTK_WIDGET(sudokuImage), GTK_ALIGN_CENTER);
  openImage("tests/image-processing-images/sudoku1.jpg", &ui);

  // Runs the main loop.
  gtk_main();

  // Exits.
  return 0;
}
