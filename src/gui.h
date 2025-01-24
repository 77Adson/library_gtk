#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "magazyn.h"

GtkWidget* create_magazyn_page(GtkStack *stack);

void show_main_menu();
void create_main_window(GtkApplication *app);
void switch_to_magazyn(GtkWidget *widget, gpointer data);
void switch_to_main(GtkWidget *widget, gpointer data);
void add_book_window();
void on_add_button_clicked(GtkWidget *widget, gpointer data);
void on_search_button_clicked(GtkWidget *button, gpointer user_data);
void on_remove_button_clicked(GtkWidget *widget, gpointer data);
void refresh_inventory(Book *inventory);


#endif // GUI_H
