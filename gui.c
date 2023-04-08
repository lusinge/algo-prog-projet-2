#include "gui.h"

static void on_search_changed(GtkSearchEntry *entry, gpointer user_data)
{
	AppData *data;

	data = (AppData *)user_data;

	/*if (data->search_text)
		free(data->search_text);*/

	strcpy(data->search_text, gtk_editable_get_text(GTK_EDITABLE(entry)));
	printf("%s\n", data->search_text);
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

static void activate_cb(GtkApplication *app, gpointer user_data)
{
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
	gtk_widget_set_valign(search_bar, GTK_ALIGN_START);
	gtk_window_set_child(GTK_WINDOW(window), search_bar);

	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
	gtk_search_bar_set_child(GTK_SEARCH_BAR(search_bar), box);

	/*Get the text typed*/
	entry = gtk_search_entry_new();
	gtk_widget_set_hexpand(entry, TRUE);
	gtk_box_append(GTK_BOX(box), entry);

	g_signal_connect(entry, "search-changed", G_CALLBACK(on_search_changed), user_data);

	g_signal_connect(search_button, "clicked",
	G_CALLBACK(on_search_button_clicked),
		     		search_bar);

	gtk_search_bar_connect_entry(GTK_SEARCH_BAR(search_bar),
				      GTK_EDITABLE(entry));
	gtk_search_bar_set_key_capture_widget(GTK_SEARCH_BAR(search_bar),
					       			window);
}

int search_window(int argc, char *argv[], AppData *data)
{
    data->app = gtk_application_new("org.gtk.Example.GtkSearchBar",
                                     G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(data->app, "activate", G_CALLBACK(activate_cb), data);

    return g_application_run(G_APPLICATION(data->app), argc, argv);
}
