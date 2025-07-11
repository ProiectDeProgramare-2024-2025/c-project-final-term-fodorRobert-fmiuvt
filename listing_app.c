/*
 * Enhanced Property Listings App
 * Features:
 * - Validated user input
 * - Colored terminal output
 * - Struct-based storage
 * - Modular functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_BUF 256
#define LISTING_FILE "listings.txt"
#define GREEN "\033[32m"
#define RED   "\033[31m"
#define CYAN  "\033[36m"
#define RESET "\033[0m"

typedef struct {
    int id;
    int price;
    int size;
    int rooms;
    int bathrooms;
    int parking;
    char date[20];
    char desc[LINE_BUF];
} Listing;

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_int(const char* prompt) {
    char buf[LINE_BUF];
    int num;
    while (1) {
        printf(GREEN "%s" RESET, prompt);
        fgets(buf, sizeof(buf), stdin);
        if (sscanf(buf, "%d", &num) == 1) return num;
        printf(RED "Invalid input! Please enter a number.\n" RESET);
    }
}

void get_string(const char* prompt, char* dest, int size) {
    printf(GREEN "%s" RESET, prompt);
    fgets(dest, size, stdin);
    dest[strcspn(dest, "\n")] = 0;
}

int validate_date_format(const char* date) {
    if (strlen(date) != 10) return 0;
    if (date[2] != '/' || date[5] != '/') return 0;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(date[i])) return 0;
    }
    return 1;
}

void add_listing() {
    Listing l;
    printf(CYAN "\n-- Add New Listing --\n" RESET);
    get_string("Date of acquisition (dd/mm/yyyy): ", l.date, sizeof(l.date));
    if (!validate_date_format(l.date)) {
        printf(RED "Invalid date format! Expected dd/mm/yyyy.\n" RESET);
        return;
    }
    l.id = get_int("ID: ");
    l.price = get_int("Price: ");
    l.size = get_int("Size: ");
    l.rooms = get_int("Rooms: ");
    l.bathrooms = get_int("Bathrooms: ");
    l.parking = get_int("Parking (0 = No, 1 = Yes): ");
    get_string("Description: ", l.desc, LINE_BUF);

    FILE* f = fopen(LISTING_FILE, "a");
    if (!f) { perror("Error opening file"); return; }
    fprintf(f, "%d,%d,%d,%d,%d,%d,%s,%s\n",
            l.id, l.price, l.size, l.rooms, l.bathrooms, l.parking, l.date, l.desc);
    fclose(f);
    printf(GREEN "Listing saved successfully!\n" RESET);
}

void view_listings() {
    FILE* f = fopen(LISTING_FILE, "r");
    if (!f) { perror("Error reading file"); return; }
    char line[LINE_BUF];
    printf(CYAN "\n-- Current Listings --\n" RESET);
    while (fgets(line, sizeof(line), f)) {
        Listing l;
        sscanf(line, "%d,%d,%d,%d,%d,%d,%19[^,],%255[^\n]",
               &l.id, &l.price, &l.size, &l.rooms,
               &l.bathrooms, &l.parking, l.date, l.desc);
        printf("ID: %s%d%s | Price: %s%d%s | Size: %d sqm | Rooms: %d | Baths: %d | Parking: %s | Date: %s | %s\n",
               CYAN, l.id, RESET,
               GREEN, l.price, RESET,
               l.size, l.rooms, l.bathrooms,
               l.parking ? "Yes" : "No", l.date, l.desc);
    }
    fclose(f);
}

int main() {
    int option;
    do {
        printf(CYAN "\n1. View Listings\n2. Add Listing\n0. Exit\n" RESET);
        option = get_int("Select an option: ");
        switch (option) {
            case 1: view_listings(); break;
            case 2: add_listing(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf(RED "Invalid option.\n" RESET);
        }
    } while (option != 0);
    return 0;
}
