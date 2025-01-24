#include "gui.h"
#include "magazyn.h"

typedef struct {
    GtkWidget *dropdown_menu;
    GtkWidget *search_bar;
} SearchWidgets;

// Function to store and retrieve the tree view reference
static GtkWidget *get_treeview_widget(GtkWidget *new_treeview) {
    static GtkWidget *treeview = NULL;
    if (new_treeview != NULL) {
        treeview = new_treeview;  // Store reference
    }
    return treeview;
}

void switch_to_magazyn(GtkWidget *widget, gpointer data) {
    GtkStack *stack = GTK_STACK(data);
    gtk_stack_set_visible_child_name(stack, "magazyn_page");
    refresh_inventory(get_inventory());
}

void switch_to_main(GtkWidget *widget, gpointer data) {
    GtkStack *stack = GTK_STACK(data);
    gtk_stack_set_visible_child_name(stack, "main_page");
}

void create_main_window(GtkApplication *app) {
    GtkWidget *main_window, *stack, *main_grid, *to_magazine_button, *load_button;

    main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "Ksiegarnia");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);

    // Create stack container
    stack = gtk_stack_new();
    gtk_container_add(GTK_CONTAINER(main_window), stack);

    // Main grid (Main Menu Page)
    main_grid = gtk_grid_new();
    gtk_stack_add_named(GTK_STACK(stack), main_grid, "main_page");

    // Buttons
    to_magazine_button = gtk_button_new_with_label("Magazyn");
    gtk_grid_attach(GTK_GRID(main_grid), to_magazine_button, 0, 0, 1, 1);

    load_button = gtk_button_new_with_label("Wczytaj");
    gtk_grid_attach(GTK_GRID(main_grid), load_button, 1, 0, 1, 1);

    // Connect buttons
    g_signal_connect(to_magazine_button, "clicked", G_CALLBACK(switch_to_magazyn), stack);
    g_signal_connect(load_button, "clicked", G_CALLBACK(load_inventory_from_file), NULL);

    // Add inventory page to stack (it will be hidden initially)
    GtkWidget *magazyn_page = create_magazyn_page(GTK_STACK(stack));
    gtk_stack_add_named(GTK_STACK(stack), magazyn_page, "magazyn_page");

    gtk_widget_show_all(main_window);
}

static GtkListStore *create_inventory_model(Book *inventory) {
    GtkListStore *store;
    GtkTreeIter iter;

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

void refresh_inventory(Book *inventory) {
    GtkWidget *treeview = get_treeview_widget(NULL);
    if (treeview == NULL) return;

    GtkListStore *store = create_inventory_model(inventory);
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);  // Free the old model
}

GtkWidget* create_magazyn_page(GtkStack *stack) {
    GtkWidget *content_area, *treeview, *scrolled_window, *back_button;
    GtkListStore *store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    /// Search container ///
    GtkWidget *dropdown_menu = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropdown_menu), "Tytul");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropdown_menu), "Autor");
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropdown_menu), 0);
    GtkWidget *search_bar = gtk_entry_new();
    GtkWidget *search_button = gtk_button_new_with_label("Szukaj");

    gtk_widget_set_size_request(search_bar, 300, 30);
    gtk_widget_set_size_request(search_button, 100, 30);
    gtk_widget_set_size_request(dropdown_menu, 100, 30);

    GtkWidget *search_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(search_container), dropdown_menu, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(search_container), search_bar, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(search_container), search_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), search_container, FALSE, FALSE, 0);

    SearchWidgets *search_widgets = malloc(sizeof(SearchWidgets));
    search_widgets->dropdown_menu = dropdown_menu;
    search_widgets->search_bar = search_bar;
    /// Search container end ///

    // Scrolled window for book list
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolled_window, 500, 300);
    gtk_box_pack_start(GTK_BOX(content_area), scrolled_window, TRUE, TRUE, 0);

    // Create tree view with list model
    store = create_inventory_model(get_inventory());
    treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store);

    // Store the tree view reference
    get_treeview_widget(treeview);

    // Add columns
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Tytuł", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    gtk_tree_view_column_set_sort_column_id(column, 0);
    
    column = gtk_tree_view_column_new_with_attributes("Autor", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    gtk_tree_view_column_set_sort_column_id(column, 1);

    column = gtk_tree_view_column_new_with_attributes("Cena", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    gtk_tree_view_column_set_sort_column_id(column, 2);

    column = gtk_tree_view_column_new_with_attributes("Ilość", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    gtk_tree_view_column_set_sort_column_id(column, 3);

    gtk_container_add(GTK_CONTAINER(scrolled_window), treeview);

    // Buttons for actions
    GtkWidget *button_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *add_button = gtk_button_new_with_label("Dodaj");
    GtkWidget *remove_button = gtk_button_new_with_label("Usuń");
    GtkWidget *edit_button = gtk_button_new_with_label("Edytuj");
    back_button = gtk_button_new_with_label("Powrót");

    gtk_box_pack_start(GTK_BOX(button_container), add_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(button_container), remove_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(button_container), edit_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(button_container), back_button, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(content_area), button_container, FALSE, FALSE, 0);

    // Connect signals
    g_signal_connect(back_button, "clicked", G_CALLBACK(switch_to_main), stack);
    g_signal_connect(add_button, "clicked", G_CALLBACK(add_book_window), NULL);
    g_signal_connect(remove_button, "clicked", G_CALLBACK(on_remove_button_clicked), NULL);
    g_signal_connect(search_button, "clicked", G_CALLBACK(on_search_button_clicked), search_widgets);

    return content_area;
}

void on_search_button_clicked(GtkWidget *button, gpointer user_data) {
    SearchWidgets *widgets = (SearchWidgets *)user_data;

    // Get selected option from dropdown menu and search text
    const char *selected_option = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets->dropdown_menu));
    const char *search_text = gtk_entry_get_text(GTK_ENTRY(widgets->search_bar));
    
    // Perform search
    Book *found_books = search(selected_option, search_text);

    // Update inventory model with search results
    refresh_inventory(found_books);

    // Free the selected option string returned by GTK
    g_free((char *)selected_option);
}

void add_book_window() {
    GtkWidget *dialog = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Dodawanie ksiazki");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 200);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry_author = gtk_entry_new();
    GtkWidget *entry_title = gtk_entry_new();
    GtkWidget *entry_price = gtk_entry_new();
    GtkWidget *entry_quantity = gtk_entry_new();
    GtkWidget *add_button = gtk_button_new_with_label("Dodaj");

    gtk_box_pack_start(GTK_BOX(content_area), gtk_label_new("Autor:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), entry_author, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), gtk_label_new("Tytul:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), entry_title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), gtk_label_new("Cena:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), entry_price, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), gtk_label_new("Ilosc:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), entry_quantity, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_area), add_button, FALSE, FALSE, 0);

    // Attach data to the button
    g_object_set_data(G_OBJECT(add_button), "entry_author", entry_author);
    g_object_set_data(G_OBJECT(add_button), "entry_title", entry_title);
    g_object_set_data(G_OBJECT(add_button), "entry_price", entry_price);
    g_object_set_data(G_OBJECT(add_button), "entry_quantity", entry_quantity);

    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_button_clicked), NULL);

    gtk_widget_show_all(dialog);
}

void on_add_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *entry_author = g_object_get_data(G_OBJECT(widget), "entry_author");
    GtkWidget *entry_title = g_object_get_data(G_OBJECT(widget), "entry_title");
    GtkWidget *entry_price = g_object_get_data(G_OBJECT(widget), "entry_price");
    GtkWidget *entry_quantity = g_object_get_data(G_OBJECT(widget), "entry_quantity");

    const char *author = gtk_entry_get_text(GTK_ENTRY(entry_author));
    const char *title = gtk_entry_get_text(GTK_ENTRY(entry_title));
    double price = atof(gtk_entry_get_text(GTK_ENTRY(entry_price)));
    int quantity = atoi(gtk_entry_get_text(GTK_ENTRY(entry_quantity)));

    add_book(author, title, price, quantity);

    // Refresh inventory after adding a book
    refresh_inventory(get_inventory());
}

// Function to handle book removal
void on_remove_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *treeview = get_treeview_widget(NULL);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *title;
        gtk_tree_model_get(model, &iter, 0, &title, -1); // Column 0 holds the book title

        // Remove the book from inventory
        remove_book(title);
        g_free(title);

        // Refresh inventory after removing the book
        refresh_inventory(get_inventory());
    } else {
        g_print("No book selected for removal.\n");
    }
}
