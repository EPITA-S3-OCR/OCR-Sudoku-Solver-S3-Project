#pragma once

#include <gtk/gtk.h>
#include "main.h"

/**
 * @brief Loads the image onto the User Interface
 * @param ui The User Interface
 * @param path The path to the file to be loaded
 */
void loadImageUi(UserInterface *ui, char *path);
void displayImage(UserInterface *ui, cairo_surface_t *surface);

/**
 * @brief Create a new Cairo surface that is a copy of the original
 * @param original The original surface
 * @return The dublicate of the original surface
 */
cairo_surface_t *duplicate_surface(cairo_surface_t *original);

/**
 * @brief Rotate a Cairo surface by a given number of degrees
 * @param surface The surface to be rotated
 * @param degrees The degrees by which the surface should be rotated
 * @return The rotated surface
 */
cairo_surface_t *rotate_surface(cairo_surface_t *surface, double degrees);
