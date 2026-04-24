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
void guardarArchivo(Item *coleccion, int cantidad);
void cargarArchivo(Item **coleccion, int *cantidad);
void menuBuscar(Item *coleccion, int cantidad);
int buscarItem(Item *coleccion, int cantidad, int tipoBusqueda, void *valor);
void modificarItem(Item *coleccion, int cantidad);

int main(int argc, char *argv[]){
    Item *coleccion = NULL;
    int cantidad = 0;

    //1. Cargar archivo (puede que no haya datos previos, o datos cargados correctamente)
    cargarArchivo(&coleccion, &cantidad);

    //2. Llamar al menu
    menu(&coleccion, &cantidad);

    //3. Liberar memoria
    free(coleccion);

    return 0;
}

//Implementacion de funciones
void menu(Item **coleccion, int *cantidad){
    int opc, n=0;

    do{
        printf("MENU\n\n");
        printf("1. Agregar Nuevo Item\n");
        printf("2. Buscar Item\n");
        printf("3. Mostrar Todos\n");
        printf("4. Modificar Item (No disponible por el momento)\n");
        printf("5. Eliminar Item (No disponible por el momento)\n");
        printf("6. Guardar y salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opc);

        switch (opc){
        case 1:
            printf("Ingrese numero de items que desea agregar: ");
            scanf("%d", &n);
            agregarItem(coleccion, cantidad, n);
            break;

        case 2:
            //printf("Buscar Item (No disponible por el momento)\n");
            menuBuscar(*coleccion, *cantidad);
            break;

        case 3:
            printf("MOSTRANDO ITEMS\n");
            mostrarItems(*coleccion, *cantidad);
            break;

        case 4:
            //printf("Modificar Item (No disponible por el momento)\n");
            modificarItem(*coleccion, *cantidad);
            break;

        case 5:
            printf("Eliminar Item (No disponible por el momento)\n");
            break;

        case 6:
            guardarArchivo(*coleccion, *cantidad);
            printf("Saliendo...\n");
            system("pause");
            break;
        
        default:
            printf("Opcion no valida. Ingrese una opcion disponible nuevamente\n");
            break;
        }
    }while(opc != 6);
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
    printf("\n%-5s %-20s %-15s %-6s %-15s %-12s\n",     //Formatear la salida de texto, despues sera modificado
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

void guardarArchivo(Item *coleccion, int cantidad){
    //Creamos un archivo binario llamado "gestionBiblioteca.dat", Modo "wb = write binary" (Escritura y binario)
    FILE *f = fopen("gestionBiblioteca.dat", "wb");     //".dat" (Data File) indica que el archivo contiene datos binarios (structs, arrays o tipos de datos numericos)
    
    //Verificacion
    if(f == NULL){
        printf("Problema al crear el archivo!!\n");
        return;
    }

    /*Los parametros para escribir con fwrite() son:
    -puntero: direccion de la estructura
    -tamaño: sizeof(struct) para asegurar que se escriben bien todos los bytes
    -cantidad: numero de elementos (en este caso la cantidad en el array de structs)
    -archivo: puntero FILE* devuelto por fopen
    Finalmente se debe usar fclose()*/
    size_t escritos = fwrite(coleccion, sizeof(Item), cantidad, f); //se usa size_t porque fwrite devuelve size_t

    //Validar que se hayan escrito todos
    if(escritos != cantidad){
        printf("Error al guardar todos los datos\n");
    }

    fclose(f);
    printf("Datos guardados correctamente\n");
}

void cargarArchivo(Item **coleccion, int *cantidad){
    //Leer el archivo binario creado llamado "gestionBiblioteca.dat", Abrir en modo "rb = read binary" (Lectura y Binario)
    FILE *f = fopen("gestionBiblioteca.dat", "rb");

    //Verificacion
    if(f == NULL){
        printf("No hay datos previos\n");
        return;
    }

    //Calcular cuantos elementos hay en el archivo
    //1. Ir al final del archivo
    fseek(f, 0, SEEK_END);

    //2. Obtener el tamaño en bytes con la funcion ftell()
    long tamaño = ftell(f);

    //3. Calcular cantidad de Items
    *cantidad = tamaño / sizeof(Item);

    //4. Regresar al inicio
    rewind(f);

    //Reservar memoria
    *coleccion = (Item*)malloc(*cantidad * sizeof(Item));
    //Verificar la creacion de memoria
    if(*coleccion == NULL){
        printf("Error al asignar memoria\n");
        fclose(f);
        return;
    }

    //Los parametros para leer con fread() son los mismos que para fwrite()
    fread(*coleccion, sizeof(Item), *cantidad, f);

    fclose(f);
    printf("Datos cargados correctamente\n");
}

int buscarItem(Item *coleccion, int cantidad, int tipoBusqueda, void *valor){
    
    for(int i = 0; i < cantidad; i++){

        switch(tipoBusqueda){
        //Buscar en ID
        case 1:
            if(coleccion[i].id == *(int*)valor)
                return i;   //Retornar posicion del arreglo de estructuras donde se haya encontrado el item
            break;

        //Buscar en Titulo
        case 2:
            if(strcmp(coleccion[i].titulo, (char*)valor) == 0)
                return i;   //Retornar posicion del arreglo de estructuras donde se haya encontrado el item
            break;

        //Buscar en Autor
        case 3:
            if(strcmp(coleccion[i].autor, (char*)valor) == 0)
                return i;   //Retornar posicion del arreglo de estructuras donde se haya encontrado el item
            break;

        //Buscar en Genero
        case 4:
            if(strcmp(coleccion[i].genero, (char*)valor) == 0)
                return i;   //Retornar posicion del arreglo de estructuras donde se haya encontrado el item
            break;

        //Buscar en Estado
        case 5:
            if(coleccion[i].estado == *(int*)valor)
                return i;   //Retornar posicion del arreglo de estructuras donde se haya encontrado el item
            break;
        }
    }

    return -1;
}

void menuBuscar(Item *coleccion, int cantidad){
    int opc;

    do{
        printf("\n=== BUSCAR ITEM ===\n");
        printf("1. Por ID\n");
        printf("2. Por Titulo\n");
        printf("3. Por Autor\n");
        printf("4. Por Genero\n");
        printf("5. Por Estado\n");
        printf("6. Regresar al menu principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opc);

        if(opc == 6){
            return;     //Regresar a menu principal
        }

        int pos = -1;

        //Buscar Item por ID
        if(opc == 1){
            int id;
            printf("Ingrese ID: ");
            scanf("%d", &id);
            pos = buscarItem(coleccion, cantidad, 1, &id);
        }

        //Buscar Item por Titulo, Autor ó Genero
        else if(opc >= 2 && opc <= 4){
            char texto[100];
            getchar();
            printf("Ingrese texto: ");
            fgets(texto, 100, stdin);
            texto[strcspn(texto, "\n")] = '\0';

            pos = buscarItem(coleccion, cantidad, opc, texto);
        }

        //Buscar Item por Estado
        else if(opc == 5){
            int estado;
            printf("Estado (0=Disponible, 1=Prestado): ");
            scanf("%d", &estado);
            pos = buscarItem(coleccion, cantidad, 5, &estado);
        }

        //Mostrar la informacion del item encontrado
        if(pos != -1){
            printf("\nItem encontrado:\n");
            printf("ID: %d\n", coleccion[pos].id);
            printf("Titulo: %s\n", coleccion[pos].titulo);
            printf("Autor: %s\n", coleccion[pos].autor);
            printf("Anio: %d\n", coleccion[pos].anio);
            printf("Genero: %s\n", coleccion[pos].genero);
            printf("Estado: %s\n", coleccion[pos].estado == DISPONIBLE ? "Disponible" : "Prestado");
        } else {
            printf("Item no encontrado\n");
        }

    }while(1);
}

void modificarItem(Item *coleccion, int cantidad){
    int id, pos;

    printf("Ingrese el ID del item a modificar: ");
    scanf("%d", &id);

    //Reutilizar la funcion buscarItem y guardar la posicion donde se encuentre el Item
    pos = buscarItem(coleccion, cantidad, 1, &id);

    if(pos == -1){
        printf("Item no encontrado\n");
        return;
    }

    //Mostrar los datos del Item encontrado
    printf("\nItem encontrado:\n");
    printf("Titulo: %s\n", coleccion[pos].titulo);
    printf("Autor: %s\n", coleccion[pos].autor);
    printf("Anio: %d\n", coleccion[pos].anio);
    printf("Genero: %s\n", coleccion[pos].genero);
    printf("Estado: %s\n", coleccion[pos].estado == DISPONIBLE ? "Disponible" : "Prestado");

    printf("\n--- Ingrese los nuevos datos ---\n");

    //Nuevo titulo
    printf("Nuevo titulo: ");
    getchar();
    fgets(coleccion[pos].titulo, TAM_TITULO, stdin);
    coleccion[pos].titulo[strcspn(coleccion[pos].titulo, "\n")] = '\0'; //Eliminar "\n" del buffer

    //Nuevo autor
    printf("Nuevo autor: ");
    fgets(coleccion[pos].autor, TAM_AUTOR, stdin);
    coleccion[pos].autor[strcspn(coleccion[pos].autor, "\n")] = '\0';   //Eliminar "\n" del buffer

    //Nuevo anio
    printf("Nuevo anio: ");
    scanf("%d", &coleccion[pos].anio);

    //Nuevo genero
    printf("Nuevo genero: ");
    getchar();
    fgets(coleccion[pos].genero, TAM_GENERO, stdin);
    coleccion[pos].genero[strcspn(coleccion[pos].genero, "\n")] = '\0';

    //Nuevo estado
    int estado;
    do{
        printf("Nuevo estado (0=Disponible, 1=Prestado): ");
        scanf("%d", &estado);
    }while(estado != 0 && estado != 1);

    coleccion[pos].estado = (Estado)estado;

    printf("Item modificado correctamente\n");

    guardarArchivo(coleccion, cantidad);    //Guardar los datos modificados
}