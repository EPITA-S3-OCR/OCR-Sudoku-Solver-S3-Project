#pragma once
#include <MagickWand/MagickWand.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include "../image-processing/main.h"
#include "../image-processing/utils/utils.h"
#include "../ocr-neural-network/main.h"
#include "../solver/solver.h"
#include "main.h"
#include "thread.h"
#include "utils.h"

// Handler for OCR Sudoku
void onImportButtonClicked(GtkButton *button, gpointer user_data);
void onRotateSliderChanged(GtkRange *range, gpointer user_data);
void onLaunchProcessButtonClicked(GtkButton *button, gpointer user_data);

// Handler for Neural Network
void onTrainButtonClicked(GtkButton *button, gpointer user_data);

// Handler for Solver
void onSolveSudokuButtonClicked(GtkButton *button, gpointer user_data);

// Handler for UI
void onWindowDestroy(GtkWidget *widget, gpointer user_data);
void radio_button_selected(GtkWidget *widget, gpointer data);