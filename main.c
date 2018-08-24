#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Category {
    struct Category *next;
    char *category_name;
    struct Zone *zone;
} *c_head;

struct Zone {
    struct Zone *next;
    char zone_type;
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


void insertCategories() {
    for(int c = 0; c < 4; c++){ // se insertan solamente 4 categorías
        // inicializacion del nodo
        struct Node *newNode = NULL;
        newNode = (struct Node*) malloc(sizeof(struct Node));
        newNode -> next = NULL;

        // inicializacion de la categoría
        struct Category *category = (struct Category*) malloc(sizeof(struct Category));
        category -> next = NULL;
        // necesario para asignar el nombre de la categoria
        switch (c) {
            case 0:
                strcpy(category -> category_name, "Norte");
                break;
            case 1:
                strcpy(category -> category_name, "Sur");
                break;
            case 2:
                strcpy(category -> category_name, "Este");
                break;
            case 3:
                strcpy(category -> category_name, "Oeste");
                break;
            default:break;
        }

        // ciclo para creacion de zonas
        for(int z = 0; z < 3; z++){
            // creacion de la zona
            struct Zone *zone = (struct Zone*) malloc(sizeof(struct Zone));
            zone -> next = NULL;
            switch (z) {
                case 0:
                    zone -> zone_type  = 'A';
                    break;
                case 2:
                    zone -> zone_type  = 'B';
                    break;
                case 3:
                    zone -> zone_type  = 'C';
                    break;
                default:break;
            }

            // ciclo para creacion de sillas
            for(int ch = 1; ch < 21; ch++){
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

            if(z_head == NULL)
                z_head = zone;
            else {
                zone -> next = z_head;
                z_head = zone;
            }
        }
        if(c_head == NULL)
            c_head = category;
        else {
            category -> next = c_head;
            c_head = category;
        }

        if(head == NULL)
            head = newNode;
        else {
            newNode -> next = head;
            head = newNode;
        }
    }
}

int main() {
    insertCategories();
    return 0;
}