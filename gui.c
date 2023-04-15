#include "gui.h"

// This function is called when the user responds to the "Enter a word" dialog
static void on_add_word_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data)
{
	// Get a pointer to the application data
	AppData *data;
	// Declare a buffer for the entered word
	char word[MAX_WORD_LENGTH];

	// If the user clicked OK
	if (response_id == GTK_RESPONSE_OK) {
		// Cast the user_data pointer to AppData*
		data = (AppData *)user_data;
		// Get the word_entry widget from the dialog's user data
		GtkWidget *entry = g_object_get_data(G_OBJECT(dialog), "word_entry");
		// Copy the text from the widget into the buffer
		strcpy(word, cut_str((char *) gtk_editable_get_text(GTK_EDITABLE(entry))));
		// Insert the word into the hash table and update the local dictionary
		//insertElementToHashTable(data->hashTab, word);
		updateLocalDictionnary(word, "mots_courants.txt");
		printf("Ajouté: %s\n", word);
	}

	// Close the dialog
	gtk_window_destroy(GTK_WINDOW(dialog));
}

// This function is called when the user responds to the "Remove a word" dialog
static void on_remove_word_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data)
{
	// Get a pointer to the application data
	AppData *data;
	// Declare a buffer for the entered word
	char word[MAX_WORD_LENGTH];

	// If the user clicked OK
	if (response_id == GTK_RESPONSE_OK) {
		// Cast the user_data pointer to AppData*
		data = (AppData *)user_data;
		// Get the word_entry widget from the dialog's user data
		GtkWidget *entry = g_object_get_data(G_OBJECT(dialog), "word_entry");
		// Copy the text from the widget into the buffer
		strcpy(word, cut_str((char *) gtk_editable_get_text(GTK_EDITABLE(entry))));
		// Insert the word into the hash table and update the local dictionary
		removeWord(data->hashTab, word, "mots_courants.txt");
		printf("Supprimé: %s\n", word);
	}

	// Close the dialog
	gtk_window_destroy(GTK_WINDOW(dialog));
}

// This function displays a "Enter a word" dialog
static void show_add_word_dialog(GtkWidget *parent, gpointer user_data)
{
	AppData *data = (AppData *)user_data;
	// Declare the widgets for the dialog
	GtkWidget *dialog, *content_area, *entry;

	// Create the dialog
	dialog = gtk_dialog_new_with_buttons("Ajout",
		  GTK_WINDOW(parent),
		  GTK_DIALOG_MODAL | GTK_DIALOG_USE_HEADER_BAR,
		  "_OK",
		  GTK_RESPONSE_OK,
		  "_Cancel",
		  GTK_RESPONSE_CANCEL,
		  NULL);

	// Get the content area of the dialog
	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	// Create an entry widget
	entry = gtk_entry_new();
	// Add the entry widget to the content area
	gtk_box_append(GTK_BOX(content_area), entry);
	// Store the entry widget in the dialog's user data
	g_object_set_data(G_OBJECT(dialog), "word_entry", entry);
	// Connect the response signal to the on_add_word_dialog_response function
	g_signal_connect(dialog, "response", G_CALLBACK(on_add_word_dialog_response), data);

	// Show the dialog
	gtk_window_present(GTK_WINDOW(dialog));
}

// This function displays a "Remove a word" dialog
static void show_remove_word_dialog(GtkWidget *parent, gpointer user_data)
{
	AppData *data = (AppData *)user_data;
	// Declare the widgets for the dialog
	GtkWidget *dialog, *content_area, *entry;

	// Create the dialog
	dialog = gtk_dialog_new_with_buttons("Suppression",
		  GTK_WINDOW(parent),
		  GTK_DIALOG_MODAL | GTK_DIALOG_USE_HEADER_BAR,
		  "_OK",
		  GTK_RESPONSE_OK,
		  "_Cancel",
		  GTK_RESPONSE_CANCEL,
		  NULL);

	// Get the content area of the dialog
	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	// Create an entry widget
	entry = gtk_entry_new();
	// Add the entry widget to the content area
	gtk_box_append(GTK_BOX(content_area), entry);
	// Store the entry widget in the dialog's user data
	g_object_set_data(G_OBJECT(dialog), "word_entry", entry);
	// Connect the response signal to the on_remove_word_dialog_response function
	g_signal_connect(dialog, "response", G_CALLBACK(on_remove_word_dialog_response), data);

	// Show the dialog
	gtk_window_present(GTK_WINDOW(dialog));
}

// This function is called when the text in the search bar changes
static void on_search_changed(GtkSearchEntry *entry, gpointer user_data)
{
	AppData *data = (AppData *)user_data;
	// Get the search text from the search entry widget
	strcpy(data->search_text, cut_str((char*) gtk_editable_get_text(GTK_EDITABLE(entry))));
	printf("%s :\n", data->search_text);
	// Find the top three words with the given prefix
	TopWord topThreeWords[3];
	findTopThreeWordsWithPrefix(data->hashTab, data->search_text, topThreeWords);
	// Print the top three words to the console
	for (int i = 0; i < 3; ++i) {
		if (topThreeWords[i].frq > 0) {
			printf("\t> %s (fréquence : %u)\n", topThreeWords[i].word, topThreeWords[i].frq);
		}
	}
	// Update the suggestion list with the top three words
	update_suggestion_list(data, topThreeWords);
}

// This function updates the suggestion list with the given top three words
static void update_suggestion_list(AppData *data, TopWord topThreeWords[3])
{
	// Remove all the current children from the list box
	GtkWidget *child;
	while ((child = gtk_widget_get_first_child(GTK_WIDGET(data->suggestion_list))) != NULL) {
		gtk_list_box_remove(data->suggestion_list, child);
	}

	// Add the new suggestions to the list box
	for (int i = 0; i < 3; ++i) {
		if (topThreeWords[i].frq > 0) {
			// Create a string for the suggestion label
			gchar *suggestion_text = g_strdup_printf("%s (fréquence : %u)",
													 topThreeWords[i].word, topThreeWords[i].frq);
			// Create a label widget with the suggestion text
			GtkWidget *suggestion_label = gtk_label_new(suggestion_text);
			// Add the label widget to the list box
			gtk_list_box_append(data->suggestion_list, suggestion_label);
			// Free the suggestion text string
			g_free(suggestion_text);
		}
	}
}

// This function is called when the search button is clicked
static void on_search_button_clicked(GtkButton *button, gpointer user_data)
{
	GtkSearchBar *search_bar = GTK_SEARCH_BAR(user_data);
	// Get the visibility status of the search bar
	gboolean visible = gtk_search_bar_get_search_mode(search_bar);
	// Toggle the visibility of the search bar
	gtk_search_bar_set_search_mode(search_bar, !visible);

	// If the search bar was just made visible, set the focus to the entry widget
	if (!visible) {
		GtkWidget *entry = gtk_widget_get_first_child(gtk_search_bar_get_child(search_bar));
		gtk_widget_grab_focus(entry);
	}
}

// This function is called when a row in the suggestion list is activated
static void on_row_activated(GtkListBox *list_box, GtkListBoxRow *row, gpointer user_data)
{
	AppData *data = (AppData *)user_data;
	// Get the label widget from the list box row
	GtkWidget *label = gtk_list_box_row_get_child(row);
	// Get the text from the label widget
	const gchar *label_text = gtk_label_get_text(GTK_LABEL(label));
	// Split the text into a word and a frequency string
	gchar **split_text = g_strsplit(label_text, " ", 2);
	gchar *word = split_text[0];
	// Insert the word into the hash table and update the local dictionary
	insertElementToHashTable(data->hashTab, word);
	updateLocalDictionnary(word, "mots_courants.txt");
	printf("la fréquence de %s a été augmentée.\n", word);
	// Free the split text array
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
	GtkWidget *plus_button;
	GtkWidget *minus_button;

	// Create the application window
	window = gtk_application_window_new(app);
	gtk_window_present(GTK_WINDOW(window));

	// Add a header bar to the window
	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

	// Add a search button to the header bar
	search_button = gtk_button_new_from_icon_name("system-search-symbolic");
	gtk_widget_set_valign(search_button, GTK_ALIGN_CENTER);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), search_button);

	// Add a plus button to the header bar for adding a new word
	plus_button = gtk_button_new_from_icon_name("value-increase-symbolic");
	gtk_widget_set_valign(plus_button, GTK_ALIGN_CENTER);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), plus_button);
	g_signal_connect_swapped(plus_button, "clicked", G_CALLBACK(show_add_word_dialog), window);
	
	// Add a minus button to the header bar for removing a new word
	minus_button = gtk_button_new_from_icon_name("value-decrease-symbolic");
	gtk_widget_set_valign(minus_button, GTK_ALIGN_CENTER);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), minus_button);
	g_signal_connect_swapped(minus_button, "clicked", G_CALLBACK(show_remove_word_dialog), window);

	// Add a search bar to the window
	search_bar = gtk_search_bar_new();

	// Add a vertical box to hold the search bar and the suggestion list
	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
	gtk_window_set_child(GTK_WINDOW(window), vbox);

	// Add the search bar to the vertical box
	gtk_box_append(GTK_BOX(vbox), search_bar);

	// Add a horizontal box to hold the search entry
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
	gtk_search_bar_set_child(GTK_SEARCH_BAR(search_bar), box);
	gtk_widget_set_valign(search_bar, GTK_ALIGN_START);

	// Add the search entry to the horizontal box
	entry = gtk_search_entry_new();
	gtk_widget_set_hexpand(entry, TRUE);
	gtk_box_append(GTK_BOX(box), entry);

	// Connect the search-changed signal to the on_search_changed function
	g_signal_connect(entry, "search-changed", G_CALLBACK(on_search_changed), user_data);

	// Connect the clicked signal of the search button to the on_search_button_clicked function
	g_signal_connect(search_button, "clicked", G_CALLBACK(on_search_button_clicked), search_bar);

	// Connect the search entry to the search bar
	gtk_search_bar_connect_entry(GTK_SEARCH_BAR(search_bar), GTK_EDITABLE(entry));

	// Set the key capture widget of the search bar to the window
	gtk_search_bar_set_key_capture_widget(GTK_SEARCH_BAR(search_bar), window);

	// Add the suggestion list to the vertical box
	GtkWidget *suggestion_list = gtk_list_box_new();
	gtk_box_append(GTK_BOX(vbox), suggestion_list);

	// Store the suggestion list in the AppData struct
	data->suggestion_list = GTK_LIST_BOX(suggestion_list);

	// Connect the row-activated signal of the suggestion list to the on_row_activated function
	g_signal_connect(suggestion_list, "row-activated", G_CALLBACK(on_row_activated), data);
}

// This function runs the search window
int search_window(int argc, char *argv[], AppData *data)
{
	// Create a new GtkApplication
	data->app = gtk_application_new("org.gtk.Example.GtkSearchBar", 0);
	// Connect the activate signal to the activate_cb function
	g_signal_connect(data->app, "activate", G_CALLBACK(activate_cb), data);
	// Run the GtkApplication and return its status code
	return g_application_run(G_APPLICATION(data->app), argc, argv);
}

void removeWord(HashTable* hashTab, char* word, char* file_name)
{
	FILE* fd;
	FILE* ftmp;
	char tmp_word[MAX_WORD_LENGTH];

	fd = fopen(file_name, "r");

	if (!fd)
		printf("\nFichier dictionnaire de prédiction non trouvé.\n");
	else {
		//copie du dico de prédiction sans le mot qui va être supprimé.
		ftmp = fopen("tmp", "w");
		while( fscanf(fd, "%s", tmp_word) != EOF ) {
			if (strcmp(tmp_word, word) != 0)
				fprintf(ftmp,"%s\n", tmp_word);
		}
		//on rééecrit le dico sans le mot voulu.
		fd = fopen(file_name, "w");
		ftmp = fopen("tmp", "r");
		strcpy(tmp_word, "");
		while( fscanf(ftmp, "%s", tmp_word) != EOF )
			fprintf(fd,"%s\n", tmp_word);

	}

}
