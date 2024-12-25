#ifndef MAGAZYN_H
#define MAGAZYN_H

#include <gtk/gtk.h>

typedef struct Book {
    char *author;
    char *title;
    double price;
    int quantity;
    struct Book *next;
} Book;

void add_book_window();
void remove_book_window();
void search_book_window();
void add_book(const char *author, const char *title, double price, int quantity);
void remove_book(const char *title);
Book* search_book(const char *title);

#endif // MAGAZYN_H
