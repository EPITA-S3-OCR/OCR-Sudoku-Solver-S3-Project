#include <gtk/gtk.h>
#include "../image-processing/main.h"
#include "main.h"

#define currentImagePath "output/current.jpg"
#define savedImagePath "output/current_saved.jpg"

#pragma once

void openImage(char *filename, userInterface *ui);

void onImportButtonClicked();

void onLaunchProcessButtonClicked();

void onManualRotationButtonClicked();

void onSudokuSizeButtonClicked();

void onDigitDetectionButtonClicked();

void onTrainNetworkButtonClicked();
