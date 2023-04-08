#include <gtk/gtk.h>

static void on_search_changed(GtkSearchEntry *entry, gpointer user_data);
static void on_search_button_clicked(GtkButton *button, gpointer user_data);
static void activate_cb(GtkApplication *app, gpointer user_data);
int search_window(int argc, char *argv[]);
