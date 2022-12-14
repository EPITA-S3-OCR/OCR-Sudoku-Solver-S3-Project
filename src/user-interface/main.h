#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <gtk/gtk.h>

/**
 * @brief The Structure for the OCR section in the UI
 * @param importButton Import Button to import a png file to solve in the UI
 * @param rotateSlider Rotates the Image
 * @param launchProcessButton Launches the process of OCR and Sudoku Solver
 * @param verboseCheckbox Check if the verbose or not
 */
typedef struct OCR
{
  GtkButton      *importButton;
  GtkScale       *rotateSlider;
  GtkButton      *launchProcessButton;
  GtkCheckButton *verboseCheckbox;

} OCR;

/**
 * @brief The Structure for the Neural Network in the UI.
 * @param trainButton Button to train the OCR Neural Network on a predefined
 * dataset
 * @param entryEpoch The Entry where the user can write the number of epochs to
 * train on.
 */
typedef struct NN
{
  GtkButton *trainButton;
  GtkEntry  *entryEpoch;
} NN;

/**
 * @brief The Structure of the Solver Section in the UI
 * @param normalSudokuRadio Button to choose to solve 9x9 Sudoku
 * @param hexadokuRadio Button to choose to solve 16x16 Sudoku
 * @param importSolverButton Button to import a sudoku txt file
 * @param sudokuPath Path to the txt sudoku file
 * @param launchProcessSolverButton Button to launch the process of solving the
 * sudoku txt file
 */
typedef struct Solver
{
  GtkRadioButton *normalSudokuRadio;
  GtkRadioButton *hexadokuRadio;
  GtkButton      *importSolverButton;
  char           *sudokuPath;
  GtkButton      *launchProcessSolverButton;
} Solver;

/**
 * @brief The Structure of the User Interface
 * @param window The window on which the User Interface is displayed
 * @param sudokuImage The Demo Sudoku image displayed
 * @param toggleImageButton Show or Hide Sudoku
 * @param stackSwitcher Switches between the different groups in the stack
 * @param stack The assembly of the different groups (OCR, Neural Network,
 * Solver)
 * @param console The console to display the process being conducted
 * @param ocr The group ocr
 * @param nn The group neural network
 * @param solver The group solver
 * @param sudokuLive The sudoku being edited
 * @param verbose Check if verbose of not
 * @param sudokuLiveSDL The surface for the Live Sudoku
 * @param consoleBuffer The buffer for the console
 */
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
  SDL_Surface      *sudokuLiveSDL;
  GtkTextBuffer    *consoleBuffer;
} UserInterface;

/**
 * @brief Adds a message to the console on the UI
 * @param data The data to be added to the console
 * @return If the message was printed
 */
gboolean addConsoleMessage(gpointer data);
#include "handler.h"
#include "thread.h"
#include "utils.h"

/**
 * @brief The main function fo the User Interface
 * @return The exit code
 */
int uiMain();
