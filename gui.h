#include <gtk/gtk.h>
#include "text.h"
#include "hash.h"

typedef struct {
  GtkApplication* app;
  char search_text[MAX_WORD_LENGTH];
  HashTable* hashTab;
}AppData;

static void on_search_changed(GtkSearchEntry *entry, gpointer user_data);
static void on_search_button_clicked(GtkButton *button, gpointer user_data);
static void activate_cb(GtkApplication *app, gpointer user_data);
int search_window(int argc, char *argv[], AppData *data);
void inc_wrd_frq(HashTable* hashTab, char* word);
