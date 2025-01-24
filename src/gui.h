#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

GtkWidget* create_magazyn_page(GtkStack *stack);
GtkWidget* create_main_menu(GtkStack *stack);
GtkWidget* get_treeview(GtkWidget *stack);



void show_main_menu();
void create_main_window(GtkApplication *app);
void switch_to_magazyn(GtkWidget *widget, gpointer data);
void switch_to_main(GtkWidget *widget, gpointer data);
void add_book_window();
void on_add_button_clicked(GtkWidget *widget, gpointer data);
void on_search_button_clicked(GtkWidget *button, gpointer user_data);
void update_inventory(GtkWidget *button, gpointer user_data);



#endif // GUI_H
