#pragma once
#include <gtk/gtk.h>
#include "../solver/main.h"
#include "handler.h"
#include "main.h"
#include "utils.h"

// void  launchExpensiveCalculation(GtkButton *button, gpointer data);

/**
 * @brief The Thread to train the neural network
 * @param user_data The Data of the User Interface
 */
void *threadTrain(void *user_data);

/**
 * @brief The Thread for the Image processing
 * @param data The data of the User Interface
 */
void *threadImageProcessing(void *data);
