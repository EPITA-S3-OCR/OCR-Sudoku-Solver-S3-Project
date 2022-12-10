#include "thread.h"

void *threadImageProcessing(void *data)
{
  printf("running image processing\n");
  UserInterface *ui = (UserInterface *)data;
  imageProcessingUi("output/ui/current.jpg", ui, ui->verbose);
  printf("image processing done\n");
  loadImageUi(ui, "output/ui/current.jpg");

  // Display the image
  displayImage(ui, ui->sudokuLive);

  // Reset slider to 0
  gtk_range_set_value(GTK_RANGE(ui->rotateSlider), 0);

  return NULL;
}

// This function will run in a separate thread
// void *run_expensive_function(void *data)
// {
//   printf("running expensive function\n");
//   UserInterface *ui = (UserInterface *)data;
//   // Emit the "update-label" signal to update the label
//   for (int i = 0; i < 1000; i++)
//   {
//     sleep(1);
//     char *message = malloc(100 * sizeof(char));
//     sprintf(message, "i = %d", i);
//     addConsoleMessage(ui, message);
//     printf("i = %d\n", i);
//     free(message);
//   }
//   return NULL;
// }
// void run_expensive_function_button(GtkWidget *button, gpointer data)
// {
//   UserInterface *ui = (UserInterface *)data;
//   // Create a new thread to run the expensive function
//   pthread_t thread;
//   pthread_create(&thread, NULL, run_expensive_function, ui);
// }
