#pragma once
#include <gtk/gtk.h>
#include <stdbool.h>
#include "../image-processing/main.h"
#include "../image-processing/utils/utils.h"
#include "main.h"
#define currentImagePath "output/ui/current.jpg"
#define savedImagePath "output/ui/current_saved.jpg"

void onImportButtonClicked(GtkButton *button, gpointer user_data);
void onRotateSliderChanged(GtkRange *range, gpointer user_data);
void onLaunchProcessButtonClicked(GtkButton *button, gpointer user_data);
void onVerboseCheckboxToggled(GtkToggleButton *togglebutton,
                              gpointer         user_data);
void onWindowDestroy(GtkWidget *widget, gpointer user_data);
