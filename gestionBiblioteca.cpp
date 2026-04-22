#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_TITULO 100
#define TAM_AUTOR 50
#define TAM_GENERO 30

enum Estado{
    DISPONIBLE,
    PRESTADO
};

struct Item{
    int id;
    char titulo[TAM_TITULO];
    char autor[TAM_AUTOR];
    int anio;
    char genero[TAM_GENERO];
    Estado estado;
};

//Prototipo de funciones
void menu(Item **coleccion, int *cantidad);
void agregarItem(Item **coleccion, int *cantidad, int n);
void mostrarItems(Item *coleccion, int cantidad);

int main(int argc, char *argv[]){
    Item *coleccion = NULL;
    int cantidad = 0;

    menu(&coleccion, &cantidad);

    return 0;
}

//Implementacion de funciones
void menu(Item **coleccion, int *cantidad){
    int opc, n=0;

    do{
        printf("Seleccione una opcion: ");
        scanf("%d", &opc);

        switch (opc){
        case 1:
            printf("Ingrese numero de items que desea agregar: ");
            scanf("%d", &n);
            agregarItem(coleccion, cantidad, n);
            break;

        case 2:
        printf("MOSTRANDO ITEMS\n");
        mostrarItems(*coleccion, *cantidad);
        
        default:
            break;
        }
    }while(opc != 5);
}

void agregarItem(Item **coleccion, int *cantidad, int n){
    int estado;

    *coleccion = (Item*)realloc(*coleccion, (*cantidad + n) * sizeof(Item));

    if(*coleccion == NULL){
        printf("Ocurrio un error al momento de reservar el espacio de memoria");
        return;
    }

    for(int i = *cantidad; i < *cantidad + n; i++){
        
        //Usuario ingresa Titulo
        printf("Ingrese el Titulo del Item: ");
        getchar();                                          //Limpiar buffer antes de fgets()
        fgets((*coleccion + i)->titulo, TAM_TITULO, stdin);
        (*coleccion + i)->titulo[strcspn((*coleccion + i)->titulo, "\n")] = '\0';   //Eliminar "\n"

        //Usuario ingresa Autor
        printf("Ingrese el Autor del Item: ");
        fgets((*coleccion + i)->autor, TAM_AUTOR, stdin);
        (*coleccion + i)->autor[strcspn((*coleccion + i)->autor, "\n")] = '\0';     //Eliminar "\n"

        //Usuario ingresa Anio
        printf("Ingrese el Anio del Item: ");
        scanf("%d", &(*coleccion + i)->anio);

        //Usuario ingresa Genero
        printf("Ingrese el Genero del Item: ");
        getchar();                                          //Limpiar buffer antes de fgets() porque antes se uso scanf("%d")
        fgets((*coleccion + i)->genero, TAM_GENERO, stdin);
        (*coleccion + i)->genero[strcspn((*coleccion + i)->genero, "\n")] = '\0';   //Eliminar "\n"

        //Usuario ingresa Estado
        //Validar que ingrese un Estado correcto
        do{
            printf("Ingrese el Estado del Item (Disponible = 0 | Prestado = 1): ");
            scanf("%d", &estado);
        }while(estado != 0 && estado != 1);
        (*coleccion + i)->estado = (Estado)estado;

        //id incrementa automaticamente
        (*coleccion + i)->id = i + 1;
    }

    *cantidad += n;
}

void mostrarItems(Item *coleccion, int cantidad){
    printf("\n%-5s %-20s %-15s %-6s %-15s %-12s\n",
            "ID", "TITULO", "AUTOR", "ANIO", "GENERO", "ESTADO");

    printf("------------------------------------------------------------------------\n");

    for(int i = 0; i < cantidad; i++){
        printf("%-5d ", (coleccion + i)->id);
        printf("%-20s ", (coleccion + i)->titulo);
        printf("%-15s ", (coleccion + i)->autor);
        printf("%-6d ", (coleccion + i)->anio);
        printf("%-15s ", (coleccion + i)->genero);

        if((coleccion + i)->estado == DISPONIBLE){
            printf("Disponible");
        }else if((coleccion + i)->estado == PRESTADO){
            printf("Prestado");
        }
    }
    printf("\n");
}