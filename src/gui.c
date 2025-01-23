#include "gui.h"
#include "magazyn.h"

static GtkListStore *create_inventory_model() {
    GtkListStore *store;
    GtkTreeIter iter;
    Book *inventory = get_inventory();

    // Create a list store with four columns: title, author, price, and quantity
    store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_DOUBLE, G_TYPE_INT);

    while (inventory != NULL) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, inventory->title,
            1, inventory->author,
            2, inventory->price,
            3, inventory->quantity,
            -1);
        inventory = inventory->next;
    }

    return store;
}

void search_window(GtkWidget *widget, gpointer data G_GNUC_UNUSED) {
    GtkWidget *dialog, *content_area, *treeview, *scrolled_window;
    GtkListStore *store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // Create dialog window
    dialog = gtk_dialog_new_with_buttons("Wyszukaj Książkę", NULL, GTK_DIALOG_MODAL, "Zamknij", GTK_RESPONSE_CLOSE, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Create a scrolled window to hold the table
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolled_window, 500, 300);
    gtk_container_add(GTK_CONTAINER(content_area), scrolled_window);

    // Create tree view with a list store model
    store = create_inventory_model();
    treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store);

    // Add columns to the tree view
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Tytuł", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    gtk_tree_view_column_set_sort_column_id(column, 0);  // Enable sorting by title

    column = gtk_tree_view_column_new_with_attributes("Autor", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    gtk_tree_view_column_set_sort_column_id(column, 1);  // Enable sorting by author

    column = gtk_tree_view_column_new_with_attributes("Cena", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    gtk_tree_view_column_set_sort_column_id(column, 2);  // Enable sorting by price

    column = gtk_tree_view_column_new_with_attributes("Ilość", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    gtk_tree_view_column_set_sort_column_id(column, 3);  // Enable sorting by quantity

    // Enable row selection highlighting
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

    // Add treeview to the scrolled window
    gtk_container_add(GTK_CONTAINER(scrolled_window), treeview);

    // Destroy dialog when closed
    g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

    gtk_widget_show_all(dialog);
}
