#include <gtk/gtk.h>
#include "magazyn.h"

static void on_add_button_clicked(GtkWidget *widget, gpointer data) {
    // Dodaj książkę do magazynu
    add_book_window();
}

static void on_remove_button_clicked(GtkWidget *widget, gpointer data) {
    // Usuń książkę z magazynu
    remove_book_window();
}

static void on_search_button_clicked(GtkWidget *widget, gpointer data) {
    // Wyszukaj książkę w magazynie
    search_book_window();
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *add_button;
    GtkWidget *remove_button;
    GtkWidget *search_button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Magazyn Księgarni");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    add_button = gtk_button_new_with_label("Dodaj Książkę");
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), add_button, 0, 0, 1, 1);

    remove_button = gtk_button_new_with_label("Usuń Książkę");
    g_signal_connect(remove_button, "clicked", G_CALLBACK(on_remove_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), remove_button, 0, 1, 1, 1);

    search_button = gtk_button_new_with_label("Wyszukaj Książkę");
    g_signal_connect(search_button, "clicked", G_CALLBACK(on_search_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), search_button, 0, 2, 1, 1);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.magazyn", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
