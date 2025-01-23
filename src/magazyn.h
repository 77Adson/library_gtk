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
void load_inventory_from_file(const char *filename);
void add_book(const char *author, const char *title, double price, int quantity);
void remove_book(const char *title);
Book* search_book(const char *title);
void free_inventory();
Book* get_inventory();

#endif // MAGAZYN_H
