#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <gtk/gtk.h>

typedef struct OCR
{
  GtkButton      *importButton;
  GtkScale       *rotateSlider;
  GtkButton      *launchProcessButton;
  GtkCheckButton *verboseCheckbox

} OCR;

typedef struct NN
{
  GtkButton *trainButton;
  GtkEntry  *entryEpoch;
} NN;

typedef struct Solver
{
  GtkCheckButton *hexaModeCheckbox;
  GtkButton      *launchProcessSolverButton;
} Solver;

typedef struct UserInterface
{
  GtkWindow        *window;
  GtkImage         *sudokuImage;
  GtkButton        *toggleImageButton;
  GtkStackSwitcher *stackSwitcher;
  GtkStack         *stack;
  GtkTextView      *console;
  OCR              *ocr;
  NN               *nn;
  Solver           *solver;
  cairo_surface_t  *sudokuLive;
  gboolean          verbose;
} UserInterface;

void addConsoleMessage(UserInterface *ui, char *message);
#include "handler.h"
#include "thread.h"
#include "utils.h"
int uiMain(int argc, char *argv[]);
