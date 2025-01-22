#include <stdio.h>

void load_inventory() {
    FILE *file = fopen("inventory.txt", "r");
    if (file == NULL) {
        printf("Nie można otworzyć pliku inventory.txt\n");
        return;
    }

    char author[100];
    char title[100];
    double price;
    int quantity;

    while (fscanf(file, "%s %s %lf %d", author, title, &price, &quantity) == 4) {
        add_book(author, title, price, quantity);
    }

    fclose(file);
}

void save_inventory() {
    FILE *file = fopen("inventory.txt", "w");
    if (file == NULL) {
        printf("Nie można otworzyć pliku inventory.txt\n");
        return;
    }

    Book *current = inventory;
    while (current != NULL) {
        fprintf(file, "%s %s %.2f %d\n", current->author, current->title, current->price, current->quantity);
        current = current->next;
    }

    fclose(file);
}