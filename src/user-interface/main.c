#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "main.h"

guint update_label_signal_id = 0;
void  addConsoleMessage(UserInterface *ui, char *message)
{
  g_signal_emit(ui->console, update_label_signal_id, 0, message);
}

void loadCSS()
{
  // add css provider from the file "mystyle.css"
  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(provider, "src/user-interface/style.css",
                                  NULL);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER);
}

gboolean update_label(GtkTextView *console, const char *text, gpointer data)
{
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(console);
  GtkTextIter    end;
  gtk_text_buffer_get_end_iter(buffer, &end);
  gtk_text_buffer_insert(buffer, &end, text, -1);
  gtk_text_buffer_insert(buffer, &end, "\n", -1);

  // if more than 3 lines in the buffer, delete the first line
  int line_count = gtk_text_buffer_get_line_count(buffer);
  if (line_count > 6)
  {
    GtkTextIter start;
    gtk_text_buffer_get_iter_at_line(buffer, &start, 0);
    gtk_text_buffer_get_iter_at_line(buffer, &end, 1);
    gtk_text_buffer_delete(buffer, &start, &end);
  }

  return FALSE;
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
      .sudokuLive          = NULL,
      .console             = console,
  };

  // Connects the signals with handler from handler.c
  g_signal_connect(window, "destroy", G_CALLBACK(onWindowDestroy), &ui);
  g_signal_connect(importButton, "clicked", G_CALLBACK(onImportButtonClicked),
                   &ui);
  //   g_signal_connect(importButton, "clicked",
  //                    G_CALLBACK(run_expensive_function_button), &ui);
  g_signal_connect(rotateSlider, "value-changed",
                   G_CALLBACK(onRotateSliderChanged), &ui);
  g_signal_connect(launchProcessButton, "clicked",
                   G_CALLBACK(onLaunchProcessButtonClicked), &ui);
  g_signal_connect(verboseCheckbox, "toggled",
                   G_CALLBACK(onVerboseCheckboxToggled), &ui);

  update_label_signal_id = g_signal_new(
      "update-label", G_TYPE_FROM_CLASS(GTK_WIDGET_GET_CLASS(console)),
      G_SIGNAL_RUN_FIRST, 0, NULL, NULL, g_cclosure_marshal_VOID__STRING,
      G_TYPE_NONE, 1, G_TYPE_STRING);

  g_signal_connect(console, "update-label", G_CALLBACK(update_label), &ui);

  // Set slider range from -180 to 180 with default value 0 and step 1
  gtk_range_set_range(GTK_RANGE(rotateSlider), -180, 180);
  gtk_range_set_value(GTK_RANGE(rotateSlider), 0);
  gtk_range_set_increments(GTK_RANGE(rotateSlider), 1, 1);

  loadImageUi(&ui, "tests/image-processing-images/sudoku1.jpg");

  //   pthread_t thread;
  //   pthread_create(&thread, NULL, run_expensive_function, NULL);

  // Runs the main loop.
  gtk_main();

  // Stop the thread
  //   pthread_cancel(thread);

  //   Exits.

  return 0;
}
