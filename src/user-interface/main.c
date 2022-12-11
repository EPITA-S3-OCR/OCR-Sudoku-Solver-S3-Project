#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "main.h"

bool           toggleImage         = false;
guint          updateLabelSignalId = 0;
GtkTextBuffer *buffer;

gboolean addConsoleMessage(gpointer data)
{
  printf("console add message\n");
  char       *message = (char *)data;
  GtkTextIter end;
  gtk_text_buffer_get_end_iter(buffer, &end);
  gtk_text_buffer_insert(buffer, &end, message, -1);
  gtk_text_buffer_insert(buffer, &end, "\n", -1);

  // if more than 3 lines in the buffer, delete the first line
  int line_count = gtk_text_buffer_get_line_count(buffer);
  if ((toggleImage == false && line_count > 6) || line_count > 30)
  {
    GtkTextIter start;
    gtk_text_buffer_get_iter_at_line(buffer, &start, 0);
    gtk_text_buffer_get_iter_at_line(buffer, &end, 1);
    gtk_text_buffer_delete(buffer, &start, &end);
  }

  return FALSE;
}

void onWebsiteButtonClicked(GtkButton *button, gpointer data)
{
  (void)button;
  (void)data;
  printf("Website Button Clicked\n");
  // test if firefox comand is available if so open in firefox else open in
  // default browser
  if (system("firefox") == 0)
  {
    system("firefox -url https://epita-s3-ocr.netlify.app");
  }
  else
  {
    system("open 'https://epita-s3-ocr.netlify.app/'");
  }
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

void onHideSudokuButtonClicked(GtkButton *button, gpointer data)
{
  printf("Hide Sudoku Button Clicked\n");
  UserInterface *ui = (UserInterface *)data;
  if (toggleImage == false)
  {
    gtk_widget_set_visible(GTK_WIDGET(ui->sudokuImage), FALSE);
    // Resize console to 500x500
    gtk_widget_set_size_request(GTK_WIDGET(ui->console), 400, 500);
    toggleImage = true;
    // set button label to "Show Sudoku"
    gtk_button_set_label(button, "Show Sudoku");
  }
  else
  {
    gtk_widget_set_visible(GTK_WIDGET(ui->sudokuImage), TRUE);
    // Resize console to 500x500
    gtk_widget_set_size_request(GTK_WIDGET(ui->console), 200, 100);
    toggleImage = false;
    // set button label to "Hide Sudoku"
    gtk_button_set_label(button, "Hide Sudoku");
  }
  // Clear console
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(ui->console);
  gtk_text_buffer_set_text(buffer, "", -1);
}

int uiMain()
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

  // Get OCR Sudoku widgets
  OCR ocr = {
      .importButton
      = GTK_BUTTON(gtk_builder_get_object(builder, "ImportButton")),
      .rotateSlider
      = GTK_SCALE(gtk_builder_get_object(builder, "RotateSlider")),
      .launchProcessButton
      = GTK_BUTTON(gtk_builder_get_object(builder, "LaunchProcessButton")),
      .verboseCheckbox
      = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "VerboseCheckbox")),
  };

  // Get Neural Network widgets
  NN nn = {
      .trainButton = GTK_BUTTON(
          gtk_builder_get_object(builder, "LaunchProcessTrainingButton")),
      .entryEpoch = GTK_ENTRY(gtk_builder_get_object(builder, "EntryEpoch")),
  };

  // Get Sudoku Solver widgets
  Solver solver = {
      .normalSudokuRadio
      = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "NormalSudokuRadio")),
      .hexadokuRadio
      = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "HexadokuRadio")),
      .launchProcessSolverButton = GTK_BUTTON(
          gtk_builder_get_object(builder, "LaunchProcessSolverButton")),
      .importSolverButton
      = GTK_BUTTON(gtk_builder_get_object(builder, "ImportSolverButton")),
  };

  UserInterface ui = {
      .window      = GTK_WINDOW(gtk_builder_get_object(builder, "window")),
      .sudokuImage = GTK_IMAGE(gtk_builder_get_object(builder, "SudokuImage")),
      .toggleImageButton
      = GTK_BUTTON(gtk_builder_get_object(builder, "ToggleImageButton")),
      .stackSwitcher
      = GTK_STACK_SWITCHER(gtk_builder_get_object(builder, "StackSwitcher")),
      .stack      = GTK_STACK(gtk_builder_get_object(builder, "Stack")),
      .console    = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "Console")),
      .ocr        = &ocr,
      .nn         = &nn,
      .solver     = &solver,
      .sudokuLive = NULL,
      .verbose    = false,
  };

  GtkButton *websiteButton
      = GTK_BUTTON(gtk_builder_get_object(builder, "WebsiteButton"));
  g_signal_connect(websiteButton, "clicked",
                   G_CALLBACK(onWebsiteButtonClicked), &ui);

  buffer = gtk_text_view_get_buffer(ui.console);
  gtk_stack_switcher_set_stack(ui.stackSwitcher, ui.stack);

  // Connects the signals with handler from handler.c
  g_signal_connect(ui.window, "destroy", G_CALLBACK(onWindowDestroy), &ui);
  g_signal_connect(ui.ocr->importButton, "clicked",
                   G_CALLBACK(onImportButtonClicked), &ui);
  g_signal_connect(ui.ocr->rotateSlider, "value-changed",
                   G_CALLBACK(onRotateSliderChanged), &ui);
  g_signal_connect(ui.ocr->launchProcessButton, "clicked",
                   G_CALLBACK(onLaunchProcessButtonClicked), &ui);

  g_signal_connect(ui.toggleImageButton, "clicked",
                   G_CALLBACK(onHideSudokuButtonClicked), &ui);

  g_signal_connect(ui.solver->importSolverButton, "clicked",
                   G_CALLBACK(onImportSolverButtonClicked), &ui);

  g_signal_connect(ui.solver->launchProcessSolverButton, "clicked",
                   G_CALLBACK(onSolveSudokuButtonClicked), &ui);

  g_signal_connect(ui.nn->trainButton, "clicked",
                   G_CALLBACK(onTrainButtonClicked), &ui);

  g_signal_connect(ui.solver->hexadokuRadio, "toggled",
                   G_CALLBACK(onHexadokuRadioToggled), &ui);
  g_signal_connect(ui.solver->normalSudokuRadio, "toggled",
                   G_CALLBACK(onNormalSudokuRadioToggled), &ui);

  // Set slider range from -180 to 180 with default value 0 and step 1
  gtk_range_set_range(GTK_RANGE(ui.ocr->rotateSlider), -180, 180);
  gtk_range_set_value(GTK_RANGE(ui.ocr->rotateSlider), 0);
  gtk_range_set_increments(GTK_RANGE(ui.ocr->rotateSlider), 1, 1);

  loadImageUi(&ui, "tests/image-processing-images/sudoku1.jpg");

  // Runs the main loop.
  gtk_main();

  return 0;
}
