#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <gtk/gtk.h>
#include "../image-processing/utils/utils.h"

#pragma once

// create a ui structure with all the widgets
struct userInterface
{
  GtkWindow *window;
  GtkButton *importButton;
  GtkButton *launchProcessButton;
  GtkButton *manualRotationButton;
  GtkButton *sudokuSizeButton;
  GtkButton *digitDetectionButton;
  GtkButton *trainNetworkButton;
  GtkImage  *sudokuImage;
} typedef userInterface;

#include "handler.h"
int uiMain(int argc, char *argv[]);
