#include <gtk/gtk.h>
#include "text.h"
#include "hash.h"

typedef struct {
  GtkApplication* app;
  GtkWidget* window;
  char search_text[MAX_WORD_LENGTH];
  HashTable* hashTab;
  GtkListBox* suggestion_list;
  GtkWidget* word_entry;
} AppData;

int search_window(int argc, char *argv[], AppData *data);
static void activate_cb(GtkApplication *app, gpointer user_data);
static void on_search_button_clicked(GtkButton *button, gpointer user_data);
static void on_search_changed(GtkSearchEntry *entry, gpointer user_data);
static void update_suggestion_list(AppData *data, TopWord topThreeWords[3]);
static void on_row_activated(GtkListBox *list_box, GtkListBoxRow *row, gpointer user_data);
static void show_add_word_dialog(gpointer user_data);
static void on_add_word_dialog_response(GtkDialog *dialog, gint response, gpointer user_data);
static void on_remove_word_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data);
static void show_remove_word_dialog(gpointer user_data);
void removeWord(HashTable* hashTab, char* word, char* file_name);
