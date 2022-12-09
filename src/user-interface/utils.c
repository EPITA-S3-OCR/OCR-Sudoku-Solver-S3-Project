#include "utils.h"

void loadImageUi(UserInterface *ui, char *path)
{
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(path, NULL);
  // Convert it to cairo surface
  cairo_surface_t *surface
      = gdk_cairo_surface_create_from_pixbuf(pixbuf, 0, NULL);
  ui->sudokuLive = surface;
  int width      = cairo_image_surface_get_width(ui->sudokuLive);
  int height     = cairo_image_surface_get_height(ui->sudokuLive);
  g_print("width: %d, height: %d\n", width, height);

  pixbuf = gdk_pixbuf_scale_simple(pixbuf, 400, 400, GDK_INTERP_BILINEAR);
  // Set the image to the pixbuf
  gtk_image_set_from_pixbuf(ui->sudokuImage, pixbuf);
  // Free the pixbuf
  g_object_unref(pixbuf);
}

void displayImage(UserInterface *ui, cairo_surface_t *surface)
{
  int        width  = cairo_image_surface_get_width(surface);
  int        height = cairo_image_surface_get_height(surface);
  GdkPixbuf *pixbuf
      = gdk_pixbuf_get_from_surface(surface, 0, 0, width, height);

  // Load an image using Pixbuf
  pixbuf = gdk_pixbuf_scale_simple(pixbuf, 400, 400, GDK_INTERP_BILINEAR);
  // Set the image to the pixbuf
  gtk_image_set_from_pixbuf(ui->sudokuImage, pixbuf);
  // Free the pixbuf
  g_object_unref(pixbuf);
}

void addConsoleMessage(UserInterface *ui, char *message)
{
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(ui->console);
  gtk_text_buffer_insert_at_cursor(buffer, message, -1);
  gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
}

// Create a new Cairo surface that is a copy of the original
cairo_surface_t *duplicate_surface(cairo_surface_t *original)
{
  int width  = cairo_image_surface_get_width(original);
  int height = cairo_image_surface_get_height(original);

  // Create a new surface with the same dimensions and content as the original
  cairo_surface_t *duplicate
      = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cairo_t *cr = cairo_create(duplicate);
  cairo_set_source_surface(cr, original, 0, 0);
  cairo_paint(cr);
  cairo_destroy(cr);

  return duplicate;
}

// Rotate a Cairo surface by a given number of degrees
cairo_surface_t *rotate_surface(cairo_surface_t *surface, double degrees)
{
  // Get the dimensions of the surface
  int width  = cairo_image_surface_get_width(surface);
  int height = cairo_image_surface_get_height(surface);
  int format = cairo_image_surface_get_format(surface);

  // Create a new surface to store the rotated image
  cairo_surface_t *rotated = cairo_image_surface_create(format, width, height);
  cairo_t         *cr      = cairo_create(rotated);

  // Translate the surface so that the rotation occurs around the center of the
  // image
  cairo_translate(cr, width / 2, height / 2);
  cairo_rotate(cr, degrees * (M_PI / 180));
  cairo_translate(cr, -width / 2, -height / 2);

  // Draw the original surface onto the rotated surface
  cairo_set_source_surface(cr, surface, 0, 0);
  cairo_paint(cr);

  // Clean up
  cairo_destroy(cr);
  cairo_surface_destroy(surface);

  // Replace the original surface with the rotated surface
  return rotated;
  // width  = cairo_image_surface_get_width(surface);
  // height = cairo_image_surface_get_height(surface);
  // g_print("width: %d, height: %d\n", width, height);
}
