#include "magazyn.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static Book *inventory = NULL;

void load_inventory_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        char *tytul = line, *autor = NULL, *cena = NULL, *ilosc = NULL;

        autor = strchr(tytul, ';');
        if (autor) {
            *autor++ = '\0';
            cena = strchr(autor, ';');
            if (cena) {
                *cena++ = '\0';
                ilosc = strchr(cena, ';');
                if (ilosc) {
                    *ilosc++ = '\0';
                }
            }
        }

        if (tytul && autor && cena && ilosc) {
            add_book(autor, tytul, atof(cena), atoi(ilosc));
        }
    }

    fclose(file);
}

Book* get_inventory() {
    return inventory;
}

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
    Book **current = &inventory;
    while (*current) {
        if (strcmp((*current)->title, title) == 0) {
            Book *to_delete = *current;
            *current = (*current)->next;
            free(to_delete->author);
            free(to_delete->title);
            free(to_delete);
            return;
        }
        current = &(*current)->next;
    }
}

Book* search_book(const char *title) {
    for (Book *current = inventory; current; current = current->next) {
        if (strcmp(current->title, title) == 0) {
            return current;
        }
    }
    return NULL;
}

void free_inventory() {
    Book *current = inventory;
    while (current) {
        Book *next = current->next;
        free(current->author);
        free(current->title);
        free(current);
        current = next;
    }
}
