#pragma once
#include <gtk/gtk.h>
#include <stdbool.h>
#include "../image-processing/main.h"
#include "../image-processing/utils/utils.h"
#include "../ocr-neural-network/main.h"
#include "../solver/solver.h"
#include "main.h"
#include "thread.h"
#include "utils.h"

void  onTrainButtonClicked(GtkButton *button, gpointer user_data);
void  onImportButtonClicked(GtkButton *button, gpointer user_data);
void  onSolveSudokuButtonClicked(GtkButton *button, gpointer user_data);
char *loadCharArray(char *path);
void  onRotateSliderChanged(GtkRange *range, gpointer user_data);
void  onLaunchProcessButtonClicked(GtkButton *button, gpointer user_data);
void  onVerboseCheckboxToggled(GtkToggleButton *togglebutton,
                               gpointer         user_data);
void  onWindowDestroy(GtkWidget *widget, gpointer user_data);
