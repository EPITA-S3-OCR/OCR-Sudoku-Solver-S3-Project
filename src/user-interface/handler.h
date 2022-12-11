#pragma once
#include <MagickWand/MagickWand.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include "../image-processing/main.h"
#include "../image-processing/utils/lines.h"
#include "../image-processing/utils/utils.h"
#include "../ocr-neural-network/main.h"
#include "../solver/solver.h"
#include "main.h"
#include "thread.h"
#include "utils.h"

// Handler for OCR Sudoku

/**
 * @brief Handler for the OCR Sudoku Import Button
 * @param button The import Button
 * @param user_data The user interface
 */
void onImportButtonClicked(GtkButton *button, gpointer user_data);

/**
 * @brief Handler for the OCR Sudoku Rotate Slider 
 * @param range The Range for the slider
 * @param user_data The user interface
 */
void onRotateSliderChanged(GtkRange *range, gpointer user_data);

/**
 * @brief Handler for the OCR Sudoku Launch Button
 * @param button The launch Button
 * @param user_data The user interface
 */
void onLaunchProcessButtonClicked(GtkButton *button, gpointer user_data);

// Handler for Neural Network

/**
 * @brief Handler for the Neural Network Train Button
 * @param button The Train Button
 * @param user_data The user interface
 */
void onTrainButtonClicked(GtkButton *button, gpointer user_data);

// Handler for Solver

/**
 * @brief Handler for the Solver Solve Button
 * @param button The Solve Button
 * @param user_data The user interface
 */
void onSolveSudokuButtonClicked(GtkButton *button, gpointer user_data);

// Handler for UI

/**
 * @brief Destroys the Window
 */
void onWindowDestroy();

/**
 * @brief Handler for the UI Hexa Toggle Button
 * @param button The Hexa Toggle Button
 * @param user_data The user interface
 */
void onHexadokuRadioToggled(GtkToggleButton *button, gpointer user_data);

/**
 * @brief Handler for the UI 9x9 Toggle Button
 * @param button The 9x9 Toggle Button
 * @param user_data The user interface
 */
void onNormalSudokuRadioToggled(GtkToggleButton *button, gpointer user_data);

/**
 * @brief Handler for the UI Import Solver Button
 * @param button The solve button
 * @param user_data The user interface
 */
void onImportSolverButtonClicked(GtkButton *button, gpointer user_data);
