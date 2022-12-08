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
  // Get widgets from the following list
  // ["ImportButton", "RotateSlider", "LaunchProcess", "VerboseCheckbox"]
  GtkButton *importButton
      = GTK_BUTTON(gtk_builder_get_object(builder, "ImportButton"));
  GtkScale *rotateSlider
      = GTK_SCALE(gtk_builder_get_object(builder, "RotateSlider"));
  GtkButton *launchProcessButton
      = GTK_BUTTON(gtk_builder_get_object(builder, "LaunchProcessButton"));
  GtkCheckButton *verboseCheckbox
      = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "VerboseCheckbox"));
  GtkImage *sudokuImage
      = GTK_IMAGE(gtk_builder_get_object(builder, "SudokuImage"));
  GtkTextView *console
      = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "Console"));

  UserInterface ui = {
      .window              = window,
      .importButton        = importButton,
      .rotateSlider        = rotateSlider,
      .launchProcessButton = launchProcessButton,
      .verboseCheckbox     = verboseCheckbox,
      .sudokuImage         = sudokuImage,
      .console             = console,
  };

  // Connects the signals with handler from handler.c
  g_signal_connect(window, "destroy", G_CALLBACK(onWindowDestroy), &ui);
  g_signal_connect(importButton, "clicked", G_CALLBACK(onImportButtonClicked),
                   &ui);
  g_signal_connect(rotateSlider, "value-changed",
                   G_CALLBACK(onRotateSliderChanged), &ui);
  g_signal_connect(launchProcessButton, "clicked",
                   G_CALLBACK(onLaunchProcessButtonClicked), &ui);
  g_signal_connect(verboseCheckbox, "toggled",
                   G_CALLBACK(onVerboseCheckboxToggled), &ui);

  // Set slider range from -180 to 180 with default value 0 and step 1
  gtk_range_set_range(GTK_RANGE(rotateSlider), -180, 180);
  gtk_range_set_value(GTK_RANGE(rotateSlider), 0);
  gtk_range_set_increments(GTK_RANGE(rotateSlider), 1, 1);
  // Solve the error for slider "drawing a gadget with negative dimensions"

  // "tests/image-processing-images/sudoku1.jpg" as image with pixbuf
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(
      "tests/image-processing-images/sudoku1.jpg", NULL);
  // Resize the pixbuf to 500x500
  pixbuf = gdk_pixbuf_scale_simple(pixbuf, 400, 400, GDK_INTERP_BILINEAR);

  gtk_image_set_from_pixbuf(sudokuImage, pixbuf);

  // Runs the main loop.
  gtk_main();

  // Exits.
  return 0;
}
