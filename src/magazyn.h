#ifndef MAGAZYN_H
#define MAGAZYN_H

#include <gtk/gtk.h>
#include <glib.h>

typedef struct Book {
    char *author;
    char *title;
    double price;
    int quantity;
    struct Book *next;
} Book;

// Function prototypes
void load_inventory_from_file();
void save_inventory_to_file();
void add_book(const char *author, const char *title, double price, int quantity);
void remove_book(const char *title);
Book* search_book(const char *title);
void free_inventory();
Book* get_inventory();
Book* search(const char *option, const char *query);
Book* copy_book(Book *inv);

#endif // MAGAZYN_H
