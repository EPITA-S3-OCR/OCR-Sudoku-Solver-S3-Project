#pragma once
#include <gtk/gtk.h>
#include "../solver/main.h"
#include "handler.h"
#include "main.h"
#include "utils.h"

void *threadTrain(void *user_data);
void *threadImageProcessing(void *data);