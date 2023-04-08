/*
 * This code is in the major part taken from
 * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/search-bar.c
 * I added a button to make the whole thing more intuitive.
 */

#include <gtk/gtk.h>
#include <gtk/gtk.h>

static void on_search_changed (GtkSearchEntry *entry, gpointer user_data)
{
	const gchar *text = gtk_egitditable_get_text (GTK_EDITABLE (entry));
	g_print ("%s\n", text);
}

static void on_search_button_clicked (GtkButton *button, gpointer user_data)
{
	GtkSearchBar *search_bar = GTK_SEARCH_BAR (user_data);
	gboolean visible = gtk_search_bar_get_search_mode (search_bar);
	gtk_search_bar_set_search_mode (search_bar, !visible);

	if (!visible) {
		GtkWidget *entry = gtk_widget_get_first_child (gtk_search_bar_get_child (search_bar));
		gtk_widget_grab_focus (entry);
	}
}

static void activate_cb (GtkApplication *app, gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *search_bar;
	GtkWidget *box;
	GtkWidget *entry;
	GtkWidget *header_bar;
	GtkWidget *search_button;

	window = gtk_application_window_new (app);
	gtk_window_present (GTK_WINDOW (window));

	header_bar = gtk_header_bar_new();
	gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

	search_button = gtk_button_new_from_icon_name("system-search-symbolic");
	gtk_widget_set_valign(search_button, GTK_ALIGN_CENTER);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), search_button);

	search_bar = gtk_search_bar_new ();
	gtk_widget_set_valign (search_bar, GTK_ALIGN_START);
	gtk_window_set_child (GTK_WINDOW (window), search_bar);

	box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
	gtk_search_bar_set_child (GTK_SEARCH_BAR (search_bar), box);

	entry = gtk_search_entry_new ();
	gtk_widget_set_hexpand (entry, TRUE);
	gtk_box_append (GTK_BOX (box), entry);

    g_signal_connect (entry, "search-changed", G_CALLBACK (on_search_changed), NULL);
    g_signal_connect (search_button, "clicked", G_CALLBACK (on_search_button_clicked), search_bar);

	gtk_search_bar_connect_entry (GTK_SEARCH_BAR (search_bar),
				      GTK_EDITABLE (entry));
	gtk_search_bar_set_key_capture_widget (GTK_SEARCH_BAR (search_bar),
					       			window);
}

int main (int argc, char *argv[])
{
	GtkApplication *app;

	app = gtk_application_new ("org.gtk.Example.GtkSearchBar",
				G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect (app, "activate", G_CALLBACK (activate_cb), NULL);

	return g_application_run (G_APPLICATION (app), argc, argv);
}
