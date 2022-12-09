#include <gtk/gtk.h>
#include "main.h"

void loadImageUi(UserInterface *ui, char *path);
void displayImage(UserInterface *ui, cairo_surface_t *surface);
void addConsoleMessage(UserInterface *ui, char *message);

// Create a new Cairo surface that is a copy of the original
cairo_surface_t *duplicate_surface(cairo_surface_t *original);

// Rotate a Cairo surface by a given number of degrees
cairo_surface_t *rotate_surface(cairo_surface_t *surface, double degrees);
