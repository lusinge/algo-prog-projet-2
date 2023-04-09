#include "gui.h"

static void on_search_changed(GtkSearchEntry *entry, gpointer user_data)
{
	AppData *data;

	data = (AppData *)user_data;

	strcpy(data->search_text, cut_str((char*) gtk_editable_get_text(GTK_EDITABLE(entry))));

	printf("%s :\n", data->search_text);

	TopWord topThreeWords[3];
	findTopThreeWordsWithPrefix(data->hashTab, data->search_text, topThreeWords);

	for (int i = 0; i < 3; ++i) {
		if (topThreeWords[i].frq > 0) {
		    printf("\t> %s (fréquence : %u)\n", topThreeWords[i].word, topThreeWords[i].frq);
		}
	}
	update_suggestion_list(data, topThreeWords);
}

static void update_suggestion_list(AppData *data, TopWord topThreeWords[3])
{
	// Remove all the current children from the list
	GtkWidget *child;
	while ((child = gtk_widget_get_first_child(GTK_WIDGET(data->suggestion_list))) != NULL) {
		gtk_list_box_remove(data->suggestion_list, child);
	}

	// Add the new suggestions to the list
	for (int i = 0; i < 3; ++i) {
		if (topThreeWords[i].frq > 0) {
		gchar *suggestion_text = g_strdup_printf("%s (fréquence : %u)",
		topThreeWords[i].word, topThreeWords[i].frq);
		GtkWidget *suggestion_label = gtk_label_new(suggestion_text);
		gtk_list_box_append(data->suggestion_list, suggestion_label);
		g_free(suggestion_text);
		}
	}
}

static void on_search_button_clicked(GtkButton *button, gpointer user_data)
{
	GtkSearchBar *search_bar = GTK_SEARCH_BAR(user_data);
	gboolean visible = gtk_search_bar_get_search_mode(search_bar);
	gtk_search_bar_set_search_mode(search_bar, !visible);

	if (!visible) {
		GtkWidget *entry = gtk_widget_get_first_child(
					gtk_search_bar_get_child(search_bar));
		gtk_widget_grab_focus(entry);
	}
}

static void on_row_activated(GtkListBox *list_box, GtkListBoxRow *row, gpointer user_data) {
	AppData *data = (AppData *)user_data;

	GtkWidget *label = gtk_list_box_row_get_child(row);
	const gchar *label_text = gtk_label_get_text(GTK_LABEL(label));
	gchar **split_text = g_strsplit(label_text, " ", 2);
	gchar *word = split_text[0];

	insertElementToHashTable(data->hashTab, word);
	updateLocalDictionnary(word, "mots_courants.txt");
	printf("la fréquence de %s a été augmentée.\n", word);
	g_strfreev(split_text);
}

static void activate_cb(GtkApplication *app, gpointer user_data)
{
	AppData *data = (AppData *)user_data;

	GtkWidget *window;
	GtkWidget *search_bar;
	GtkWidget *box;
	GtkWidget *entry;
	GtkWidget *header_bar;
	GtkWidget *search_button;

	/*Making a blank window*/
	window = gtk_application_window_new(app);
	gtk_window_present(GTK_WINDOW(window));

	/*Adding a header bar, and a pretty search button*/
	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

	search_button = gtk_button_new_from_icon_name("system-search-symbolic");
	gtk_widget_set_valign(search_button, GTK_ALIGN_CENTER);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), search_button);

	/*Adding a search bar*/
	search_bar = gtk_search_bar_new();

	/*Add a vertical box to hold the search bar and the list box*/
	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
	gtk_window_set_child(GTK_WINDOW(window), vbox);

	/*Add the search bar to the vertical box*/
	gtk_box_append(GTK_BOX(vbox), search_bar);

	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
	gtk_search_bar_set_child(GTK_SEARCH_BAR(search_bar), box);
	gtk_widget_set_valign(search_bar, GTK_ALIGN_START); // Move this line here

	/*Get the text typed*/
	entry = gtk_search_entry_new();
	gtk_widget_set_hexpand(entry, TRUE);
	gtk_box_append(GTK_BOX(box), entry);

	g_signal_connect(entry, "search-changed", G_CALLBACK(on_search_changed),
	user_data);

	g_signal_connect(search_button, "clicked",
G_CALLBACK(on_search_button_clicked), search_bar);

	gtk_search_bar_connect_entry(GTK_SEARCH_BAR(search_bar),
					GTK_EDITABLE(entry));
	gtk_search_bar_set_key_capture_widget(GTK_SEARCH_BAR(search_bar),
								window);

	/*Add the list box to the vertical box*/
	GtkWidget *suggestion_list = gtk_list_box_new();
	gtk_box_append(GTK_BOX(vbox), suggestion_list);

	/*Store the list box in the AppData struct*/
	data->suggestion_list = GTK_LIST_BOX(suggestion_list);

	g_signal_connect(suggestion_list, "row-activated", G_CALLBACK(on_row_activated), data);

}

int search_window(int argc, char *argv[], AppData *data)
{
    data->app = gtk_application_new("org.gtk.Example.GtkSearchBar",
                                     G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(data->app, "activate", G_CALLBACK(activate_cb), data);

    return g_application_run(G_APPLICATION(data->app), argc, argv);
}
