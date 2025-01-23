#include "gui.h"
#include "magazyn.h"

void switch_to_magazyn(GtkWidget *widget, gpointer data) {
    GtkStack *stack = GTK_STACK(data);
    gtk_stack_set_visible_child_name(stack, "magazyn_page");
}

void switch_to_main(GtkWidget *widget, gpointer data) {
    GtkStack *stack = GTK_STACK(data);
    gtk_stack_set_visible_child_name(stack, "main_page");
}


void create_main_window(GtkApplication *app) {
    GtkWidget *main_window, *stack, *main_grid, *search_button;

    main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "Księgarnia");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);

    // Create stack container
    stack = gtk_stack_new();
    gtk_container_add(GTK_CONTAINER(main_window), stack);

    // Main grid (Main Menu Page)
    main_grid = gtk_grid_new();
    gtk_stack_add_named(GTK_STACK(stack), main_grid, "main_page");

    // Search button (to switch to magazyn page)
    search_button = gtk_button_new_with_label("Magazyn");
    gtk_grid_attach(GTK_GRID(main_grid), search_button, 0, 0, 1, 1);

    // Connect button to switch pages
    g_signal_connect(search_button, "clicked", G_CALLBACK(switch_to_magazyn), stack);

    // Add inventory page to stack (it will be hidden initially)
    GtkWidget *magazyn_page = create_magazyn_page(GTK_STACK(stack));
    gtk_stack_add_named(GTK_STACK(stack), magazyn_page, "magazyn_page");

    gtk_widget_show_all(main_window);
}

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

GtkWidget* create_magazyn_page(GtkStack *stack) {
    GtkWidget *content_area, *treeview, *scrolled_window, *back_button;
    GtkListStore *store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    /// Search container ///
    GtkWidget *dropdown_menu = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropdown_menu), "Tytuł");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropdown_menu), "Autor");
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

    // Scrolled window for book list
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolled_window, 500, 300);
    gtk_box_pack_start(GTK_BOX(content_area), scrolled_window, TRUE, TRUE, 0);

    // Create tree view with list model
    store = create_inventory_model();
    treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store);

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

    return content_area;
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
}
