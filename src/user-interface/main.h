#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <gtk/gtk.h>

typedef struct UserInterface
{
  GtkWindow       *window;
  GtkButton       *importButton;
  GtkScale        *rotateSlider;
  GtkButton       *launchProcessButton;
  GtkCheckButton  *verboseCheckbox;
  GtkButton       *solveSudokuButton;
  GtkImage        *sudokuImage;
  GtkTextView     *console;
  cairo_surface_t *sudokuLive;
  gboolean         verbose;
} UserInterface;

void addConsoleMessage(UserInterface *ui, char *message);
#include "handler.h"
#include "thread.h"
#include "utils.h"
int uiMain(int argc, char *argv[]);
