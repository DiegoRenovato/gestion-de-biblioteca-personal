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

int main(int argc, char *argv[]){
    Item *coleccion = NULL;
    int cantidad = 0;


    return 0;
}
