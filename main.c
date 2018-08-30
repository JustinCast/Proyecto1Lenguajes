#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <conio.h>

#define FILE_NAME "result.txt"

struct Category {
    //struct Category *next;
    char *category_name;
    struct Zone *zone;
    int available_spaces;
    int current_seat;
};

struct Zone {
    struct Zone *next;
    struct Zone *prev;
    char zone_type;
    int is_full;
    int available_seats;
    struct Chair *chair;
} *z_head;

struct Chair {
    struct Chair *next;
    int chair_number;
    int row_number;
    int status;
} *ch_head;

struct Node {
    struct Node *next;
    struct Category *category;
} *head;


void insert_categories() {
    for(int c = 0; c < 4; c++){ // se insertan solamente 4 categorías
        // inicializacion del nodo
        struct Node *newNode = NULL;
        newNode = (struct Node*) malloc(sizeof(struct Node));
        newNode -> next = NULL;

        // inicializacion de la categoría
        struct Category *category = (struct Category*) malloc(sizeof(struct Category));
        category -> available_spaces = 60;
        category -> current_seat = 1;
        // necesario para asignar el nombre de la categoria
        category -> category_name = (char*) malloc(sizeof(char) * 12);
        switch (c) {
            case 0:
                strcpy(category -> category_name, "Tribuna");
                break;
            case 1:
                strcpy(category -> category_name, "Norte");
                break;
            case 2:
                strcpy(category -> category_name, "Sur");
                break;
            case 3:
                strcpy(category -> category_name, "Platea");
                break;
            default:break;
        }

        // ciclo para creacion de zonas
        for(int z = 0; z < 3; z++){
            // creacion de la zona
            struct Zone *zone = (struct Zone*) malloc(sizeof(struct Zone));
            zone -> next = NULL;
            zone -> prev = NULL;
            zone -> is_full = 0;
            zone -> available_seats = 20;
            switch (z) {
                case 0:
                    zone -> zone_type = 'C';
                    break;
                case 1:
                    zone -> zone_type = 'B';
                    break;
                case 2:
                    zone -> zone_type = 'A';
                    break;
                default:break;
            }


            // ciclo para creacion de sillas
            for(int ch = 20; ch > 0; ch--){
                struct Chair *chair = (struct Chair*) malloc(sizeof(struct Chair));
                chair -> next = NULL;

                chair -> chair_number = ch;
                chair -> status = 0;
                if(ch < 11)
                    chair -> row_number = 1;
                else
                    chair -> row_number = 2;

                // insercion
                if(ch_head == NULL)
                    ch_head = chair;
                else{
                    chair -> next = ch_head;
                    ch_head = chair;
                }
            }

            zone -> chair = ch_head;
            ch_head = NULL;
            // apunta a la cabeza
            if(z_head == NULL)
                z_head = zone;
            else {
                z_head -> prev = zone;
                zone -> next = z_head;
                z_head = zone;
            }
        }

        // apunta a la cabeza
        category -> zone = z_head;
        z_head = NULL;

        newNode -> category = category;
        if(head == NULL)
            head = newNode;
        else {
            newNode -> next = head;
            head = newNode;
        }
    }
}

void resolve_purchase_request(int tickets, char * category) {
    FILE* file_ptr = fopen(FILE_NAME, "a");
    struct Node *iterator = head;
    char *result = (char*) malloc(1);
    while(iterator != NULL){
        if(strcmp(iterator -> category -> category_name, category) == 0){
            // el puntero esta ubicado en la categoria correcta
            if(iterator -> category -> available_spaces < tickets) {
                printf("La cantidad de ticketes es menor a la solicitada\n");
                fclose(file_ptr);
                return;
            }
            fprintf(file_ptr, "%s:", iterator -> category -> category_name);
            goto CONTINUE;
        }
        iterator = iterator -> next;
    } // zona revisada: funciona correctamente

    CONTINUE:
    while((iterator -> category -> zone != NULL) && (tickets > 0)) {
        if (iterator->category->zone->is_full == 1) {
            while (iterator->category->zone->is_full == 1)
                iterator->category->zone = iterator->category->zone->next; // si la zona está llena se desplazará hasta encontrar una vacia
        }
        else {
            while (iterator->category->zone->chair != NULL && (tickets > 0)) {
                if (iterator->category->zone->chair->status == 0) {

                    iterator->category->zone->chair->status = 1;
                    printf("Silla: %i\n", iterator->category->zone->chair->chair_number);

                    iterator->category->available_spaces--;
                    iterator -> category -> zone -> available_seats--;
                    tickets--;

                    sprintf(result, "%d", iterator -> category -> current_seat);
                    fprintf(file_ptr, "%s", result);
                    iterator -> category -> current_seat++;

                    if (iterator->category->zone->chair != NULL && (tickets > 0))
                        fprintf(file_ptr, "%c", '-');
                    iterator->category->zone->chair = iterator->category->zone->chair->next;
                }
            }
            if(iterator -> category -> zone -> available_seats == 0)
                iterator -> category -> zone -> is_full = 1;
        }
    }
    fprintf(file_ptr, "\n");
    fclose(file_ptr);
}

int main(int argc, char* category[]) {
    insert_categories();
    resolve_purchase_request(atoi(category[3]), category[4]);
    resolve_purchase_request(atoi(category[1]), category[2]);
    return 0;
}