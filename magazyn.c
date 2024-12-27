#include "magazyn.h"
#include <stdlib.h>
#include <string.h>
// #include <gsignal.h>

static Book *inventory = NULL;

void add_book(const char *author, const char *title, double price, int quantity) {
    Book *new_book = malloc(sizeof(Book));
    new_book->author = strdup(author);
    new_book->title = strdup(title);
    new_book->price = price;
    new_book->quantity = quantity;
    new_book->next = inventory;
    inventory = new_book;
}

void remove_book(const char *title) {
    Book *current = inventory;
    Book *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->title, title) == 0) {
            if (prev == NULL) {
                inventory = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->author);
            free(current->title);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

Book* search_book(const char *title) {
    Book *current = inventory;
    while (current != NULL) {
        if (strcmp(current->title, title) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void add_book_window() {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *author_entry, *title_entry, *price_entry, *quantity_entry;
    GtkWidget *submit_button;

    dialog = gtk_dialog_new_with_buttons("Dodaj Książkę", NULL, GTK_DIALOG_MODAL, "Zamknij", GTK_RESPONSE_CLOSE, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Autor:"), 0, 0, 1, 1);
    author_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), author_entry, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Tytuł:"), 0, 1, 1, 1);
    title_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), title_entry, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Cena:"), 0, 2, 1, 1);
    price_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), price_entry, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Ilość:"), 0, 3, 1, 1);
    quantity_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), quantity_entry, 1, 3, 1, 1);

    submit_button = gtk_button_new_with_label("Dodaj");
    g_signal_connect(submit_button, "clicked", G_CALLBACK(add_book_button_clicked), (gpointer)dialog);
    gtk_grid_attach(GTK_GRID(grid), submit_button, 0, 4, 2, 1);

    g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

    gtk_widget_show_all(dialog);
}

void remove_book_window() {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *title_entry;
    GtkWidget *submit_button;

    dialog = gtk_dialog_new_with_buttons("Usuń Książkę", NULL, GTK_DIALOG_MODAL, "Zamknij", GTK_RESPONSE_CLOSE, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Tytuł do usunięcia:"), 0, 0, 1, 1);
    title_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), title_entry, 1, 0, 1, 1);

    submit_button = gtk_button_new_with_label("Usuń");
    g_signal_connect(submit_button, "clicked", G_CALLBACK(remove_book_button_clicked), title_entry);
    gtk_grid_attach(GTK_GRID(grid), submit_button, 0, 1, 2, 1);

    g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

    gtk_widget_show_all(dialog);
}

void search_book_window() {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *title_entry;
    GtkWidget *search_button;

    dialog = gtk_dialog_new_with_buttons("Wyszukaj Książkę", NULL, GTK_DIALOG_MODAL, "Zamknij", GTK_RESPONSE_CLOSE, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Tytuł do wyszukania:"), 0, 0, 1, 1);
    title_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), title_entry, 1, 0, 1, 1);

    search_button = gtk_button_new_with_label("Wyszukaj");
    g_signal_connect(search_button, "clicked", G_CALLBACK(search_book_button_clicked), title_entry);
    gtk_grid_attach(GTK_GRID(grid), search_button, 0, 1, 2, 1);

    g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

    gtk_widget_show_all(dialog);
}

void add_book_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = GTK_WIDGET(data);
    GtkWidget *grid = gtk_bin_get_child(GTK_BIN(gtk_dialog_get_content_area(GTK_DIALOG(dialog))));

    const char *author = gtk_entry_get_text(GTK_ENTRY(gtk_grid_get_child_at(GTK_GRID(grid), 1, 0)));
    const char *title = gtk_entry_get_text(GTK_ENTRY(gtk_grid_get_child_at(GTK_GRID(grid), 1, 1)));
    const char *price_str = gtk_entry_get_text(GTK_ENTRY(gtk_grid_get_child_at(GTK_GRID(grid), 1, 2)));
    const char *quantity_str = gtk_entry_get_text(GTK_ENTRY(gtk_grid_get_child_at(GTK_GRID(grid), 1, 3)));

    double price = atof(price_str);
    int quantity = atoi(quantity_str);

    if (author && title && price > 0 && quantity > 0) {
        add_book(author, title, price, quantity);
    }

    gtk_widget_destroy(dialog);
}

void remove_book_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *title_entry = GTK_WIDGET(data);
    const char *title = gtk_entry_get_text(GTK_ENTRY(title_entry));

    if (title && strlen(title) > 0) {
        remove_book(title);
    }
}

void search_book_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *title_entry = GTK_WIDGET(data);
    const char *title = gtk_entry_get_text(GTK_ENTRY(title_entry));

    if (title && strlen(title) > 0) {
        Book *found = search_book(title);
        if (found) {
            g_print("Znaleziono: %s, Autor: %s, Cena: %.2f, Ilość: %d\n", found->title, found->author, found->price, found->quantity);
        } else {
            g_print("Nie znaleziono książki o tytule: %s\n", title);
        }
    }
}
