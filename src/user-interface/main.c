#include "main.h"

void loadCSS()
{
  // add css provider from the file "mystyle.css"
  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(provider, "mystyle.css", NULL);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER);
}

int uiMain(int argc, char *argv[])
{
  // Initializes GTK.
  gtk_init(NULL, NULL);

  // Load CSS
  loadCSS();

  // Loads the UI description and builds the UI.
  // (Exits if an error occurs.)
  GtkBuilder *builder = gtk_builder_new();
  GError     *error   = NULL;
  if (gtk_builder_add_from_file(builder, "ui.glade", &error) == 0)
  {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return 1;
  }

  // Gets the widgets.
  GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
  GtkButton *start_button
      = GTK_BUTTON(gtk_builder_get_object(builder, "button-one"));

  // Connects signal handlers.
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  // g_signal_connect(start_button, "clicked", G_CALLBACK(on_start), NULL);

  gtk_widget_show(GTK_WIDGET(window));

  // Runs the main loop.
  gtk_main();

  // Exits.
  return 0;
}
