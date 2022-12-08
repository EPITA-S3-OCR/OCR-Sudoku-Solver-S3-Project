#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <gtk/gtk.h>
#include "handler.h"

typedef struct UserInterface
{
  GtkWindow      *window;
  GtkButton      *importButton;
  GtkScale       *rotateSlider;
  GtkButton      *launchProcessButton;
  GtkCheckButton *verboseCheckbox;
  GtkImage       *sudokuImage;
  GtkTextView    *console;
} UserInterface;

int uiMain(int argc, char *argv[]);
