#include "magazyn.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static Book *inventory = NULL;

void load_inventory_from_file() {
    // Free the current inventory before loading a new one
    free_inventory();

    FILE *file = fopen("src/savefile.txt", "r");
    if (file == NULL) {
        printf("Error opening savefile\n");
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

void save_inventory_to_file() {
    FILE *file = fopen("src/savefile.txt", "w");
    if (file == NULL) {
        printf("Error opening savefile\n");
        return;
    }

    Book *current = inventory;
    while (current) {
        fprintf(file, "%s;%s;%f;%d\n", current->title, current->author, current->price, current->quantity);
        current = current->next;
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
    printf("Added book: %s\n", title);
}

Book* search(const char *option, const char *query) {
    if (query == NULL || strlen(query) == 0) {
        return get_inventory();  // Return the full inventory if the query is empty or NULL
    }

    Book *found_inventory = NULL;  // Start with an empty list
    Book *current = get_inventory();
    printf("Starting loop\n");
    if (strcmp(option, "Tytul") == 0){
        printf("Searching by title\n");
        while (current) {
            printf("Book: %s\n", current->title);
            if (strcmp(current->title, query) == 0){
                printf("FOund title\n");
                Book *found_book = malloc(sizeof(Book));
                *found_book = *current;
                found_book->next = found_inventory;
                found_inventory = found_book;
            }
            current = current->next;
        }
    } else {
        printf("Searching by author\n");
        while (current) {
            printf("Author: %s\n", current->author);
            if (strcmp(current->author, query) == 0){
                printf("Found author\n");
                Book *found_book = malloc(sizeof(Book));
                *found_book = *current;
                found_book->next = found_inventory;
                found_inventory = found_book;
            }
            current = current->next;
        }
    }
    printf("Loop finished\n");
    return found_inventory;
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

void free_inventory() {
    Book *current = inventory;
    while (current) {
        Book *next = current->next;
        free(current->author);
        free(current->title);
        free(current);
        current = next;
    }
    inventory = NULL;
}
