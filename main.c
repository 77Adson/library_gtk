#include <gtk/gtk.h>
#include "magazyn.h"

/**
 * Callback function for the "Add" button click event.
 *
 * This function is triggered when the user clicks the "Add" button.
 * It opens a window with a form to add a new book to the inventory.
 *
 * @param widget The button widget that triggered the event.
 * @param data Additional data passed to the callback function (unused).
 */

static void on_add_button_clicked(GtkWidget *widget, gpointer data) {
    // Dodaj książkę do magazynu
    add_book_window();
}

/**
 * Callback function for the "Remove" button click event.
 *
 * This function is triggered when the user clicks the "Remove" button.
 * It opens a window with a form to remove a book from the inventory.
 *
 * @param widget The button widget that triggered the event.
 * @param data Additional data passed to the callback function (unused).
 */

static void on_remove_button_clicked(GtkWidget *widget, gpointer data) {
    // Usuń książkę z magazynu
    remove_book_window();
}

/**
 * Callback function for the "Search" button click event.
 *
 * This function is triggered when the user clicks the "Search" button.
 * It opens a window with a form to search for a book in the inventory.
 *
 * @param widget The button widget that triggered the event.
 * @param data Additional data passed to the callback function (unused).
 */
static void on_search_button_clicked(GtkWidget *widget, gpointer data) {
    // Wyszukaj książkę w magazynie
    search_book_window();
}

/**
 * Główna funkcja programu, uruchamiana po utworzeniu obiektu GtkApplication.
 *
 * Funkcja tworzy okno główne programu z trzema guzikami:
 * "Dodaj Książkę", "Usuń Książkę", "Wyszukaj Książkę". Gdy użytkownik
 * kliknie jeden z tych guzików, to odpowiednia funkcja obsługi zdarzenia
 * jest wywoływana.
 *
 * @param app obiekt GtkApplication, który uruchomił tę funkcję
 * @param user_data wskaźnik na dodatkowe dane przekazywane do tej funkcji
 *                  (nie używany)
 */
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

/**
 * Główna funkcja programu, uruchamiana automatycznie po uruchomieniu
 * programu.
 *
 * Funkcja tworzy obiekt GtkApplication i  ączy go z funkcją
 * activate, która jest wywoływana, gdy aplikacja jest uruchamiana.
 * Funkcja uruchamia pętlę zdarzeń Gliba i zwraca status końcowy
 * aplikacji.
 *
 * @param argc liczba argumentów wiersza poleceń
 * @param argv tablica argumentów wiersza poleceń
 * @return status końcowy aplikacji
 */
int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.magazyn", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
