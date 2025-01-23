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

void magazin_window(GtkWidget *widget, gpointer data G_GNUC_UNUSED) {
    GtkWidget *dialog, *content_area, *treeview, *scrolled_window;
    GtkListStore *store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // Create dialog window
    dialog = gtk_dialog_new_with_buttons("Wyszukaj Książkę", NULL, GTK_DIALOG_MODAL, "Zamknij", GTK_RESPONSE_CLOSE, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 600, 400);

    /// Search container ///
    GtkWidget *dropdown_menu = gtk_combo_box_text_new();    // Create dropdown menu
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropdown_menu), "Tytuł");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropdown_menu), "Autor");
    GtkWidget *search_bar = gtk_entry_new();    // Create search bar
    GtkWidget *search_button = gtk_button_new_with_label("Szukaj"); // Create search button

    gtk_widget_set_size_request(search_bar, 300, 30);
    gtk_widget_set_size_request(search_button, 100, 30);
    gtk_widget_set_size_request(dropdown_menu, 100, 30);

    GtkWidget *search_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); // Create container for search elements
    gtk_box_pack_start(GTK_BOX(search_container), dropdown_menu, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(search_container), search_bar, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(search_container), search_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), search_container, FALSE, FALSE, 0);   // Add container to dialog
    /// End of search container ///



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

    // Buttons
    GtkWidget *add_button = gtk_button_new_with_label("Dodaj");
    GtkWidget *remove_button = gtk_button_new_with_label("Usuń");
    GtkWidget *edit_button = gtk_button_new_with_label("Edytuj");

    // Button settings
    GtkWidget *button_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_size_request(add_button, 100, 30);
    gtk_widget_set_size_request(remove_button, 100, 30);
    gtk_widget_set_size_request(edit_button, 100, 30);

    // Add buttons to the dialog
    gtk_box_pack_start(GTK_BOX(button_container), add_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(button_container), remove_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(button_container), edit_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), button_container, FALSE, FALSE, 0);

    // Connect signals to buttons
    g_signal_connect(add_button, "clicked", G_CALLBACK(add_book_window), NULL);
    // g_signal_connect(remove_button, "clicked", G_CALLBACK(remove_book), NULL);
    // g_signal_connect(edit_button, "clicked", G_CALLBACK(edit_book), NULL);

    // Destroy dialog when closed
    g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

    gtk_widget_show_all(dialog);
}

void add_book_window(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Dodawanie ksiazki");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 200);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label_author = gtk_label_new("Autor:");
    GtkWidget *label_title = gtk_label_new("Tytul:");
    GtkWidget *label_price = gtk_label_new("Cena:");
    GtkWidget *label_quantity = gtk_label_new("Ilosc:");
    GtkWidget *entry_author = gtk_entry_new();
    GtkWidget *entry_title = gtk_entry_new();
    GtkWidget *entry_price = gtk_entry_new();
    GtkWidget *entry_quantity = gtk_entry_new();

    GtkWidget *button_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *add_button = gtk_button_new_with_label("Dodaj");

    gtk_box_pack_start(GTK_BOX(button_container), add_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), label_author, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), entry_author, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), label_title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), entry_title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), label_price, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), entry_price, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), label_quantity, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), entry_quantity, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), button_container, FALSE, FALSE, 0);

    g_signal_connect(add_button, "clicked", G_CALLBACK(add_book), NULL);
    gtk_widget_show_all(dialog);
}