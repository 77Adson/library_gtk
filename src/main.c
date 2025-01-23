#include <gtk/gtk.h>
#include "magazyn.h"
#include "gui.h"

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *main_window, *main_grid;

    main_window = gtk_application_window_new(app);
    // main window settings
    gtk_window_set_title(GTK_WINDOW(main_window), "Księgarnia");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);

    // main grid
    main_grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(main_window), main_grid);

    // search button
    GtkWidget *search_button = gtk_button_new_with_label("Magazyn");
    gtk_grid_attach(GTK_GRID(main_grid), search_button, 0, 0, 1, 1);
    g_signal_connect(search_button, "clicked", G_CALLBACK(magazin_window), NULL);

    // show all widgets
    gtk_widget_show_all(main_window);

}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.gtk.magazyn", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    load_inventory_from_file("src/savefile.txt");
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    free_inventory();

    return status;
}
