#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <windows.h>
#include <iomanip>
#include <time.h>

using namespace std;

#define TAM_TITULO 100
#define TAM_AUTOR 50
#define TAM_GENERO 30

enum Estado{
    DISPONIBLE,
    PRESTADO
};

struct fechas{
    int dia;
    int mes;
    int year;
};

struct Item{
    int id;
    char titulo[TAM_TITULO];
    char autor[TAM_AUTOR];
    int anio;
    char genero[TAM_GENERO];
    Estado estado;
    fechas fecha;
};

//Prototipo de funciones
void menu(Item **coleccion, int *cantidad);
void mostrarProgreso(double);
void pausa();
void cicloProgreso();
void agregarItem(Item **coleccion, int *cantidad, int n);
void mostrarItems(Item *coleccion, int cantidad);
void guardarArchivo(Item *coleccion, int cantidad);
void cargarArchivo(Item **coleccion, int *cantidad);
void menuBuscar(Item *coleccion, int cantidad);
int buscarItem(Item *coleccion, int cantidad, int tipoBusqueda, void *valor);
void buscarItem2(Item *coleccion, int cantidad, int tipoBusqueda, void *valor);
void modificarItem(Item *coleccion, int cantidad);
void eliminarItem(Item **coleccion, int *cantidad);
void buzz(Item*, int);
void imprimirEncabezado(string titulo = "RESULTADO BUSQUEDA");
void reportes(Item*, int, int, int);
void menuReportes(Item*, int);
string nombreMes(int);

int main(int argc, char *argv[]){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Item *coleccion = NULL;
    int cantidad = 0;

    cicloProgreso();

    //1. Cargar archivo (puede que no haya datos previos, o datos cargados correctamente)
    cargarArchivo(&coleccion, &cantidad);
    cout << "Presione ENTER para continuar...";
    getchar();

    //2. Llamar al menu
    menu(&coleccion, &cantidad);

    //3. Liberar memoria
    free(coleccion);

    getchar(); getchar();

    return 0;
}

//Implementacion de funciones
void menu(Item **coleccion, int *cantidad){
    int opc, n=0;

    do{
        system("cls || clear");
        system("color 0B");
        //system("color 02"); -> verde
        //system("color 03"); -> aqua
        //system("color 0A"); -> verde claro
        //system("color 0B"); -> aqua claro
        //system("color 0E"); -> amarillo claro
        cout << "╔══════════════════════════════════╗\n";
        cout << "║      BIBLIOTECA PERSONAL         ║\n";
        cout << "╠══════════════════════════════════╣\n";
        cout << "║  1) Agregar Nuevo Item           ║\n";
        cout << "║  2) Buscar Item                  ║\n";
        cout << "║  3) Mostrar Todos                ║\n";
        cout << "║  4) Modificar Item               ║\n";
        cout << "║  5) Eliminar Item                ║\n";
        cout << "║  6) Reportes                     ║\n";
        cout << "║  7) Guardar y Salir              ║\n";
        cout << "╠══════════════════════════════════╣\n";
        cout << "║  Seleccione una opción:          ║\n";
        cout << "╚══════════════════════════════════╝\n";
        cout << "➜ ";
        scanf("%d", &opc);

        switch (opc){
        case 1:
            printf("\nIngrese numero de items que desea agregar: ");
            scanf("%d", &n);
            system("cls || clear");
            agregarItem(coleccion, cantidad, n);
            break;

        case 2:
            //printf("Buscar Item (No disponible por el momento)\n");
            menuBuscar(*coleccion, *cantidad);
            break;

        case 3:
            system("cls || clear");
            cicloProgreso();
            cout << "\n╔══════════════════════════════════╗\n";
            cout << "║         MOSTRANDO ITEMS          ║\n";
            cout << "╚══════════════════════════════════╝\n";
            mostrarItems(*coleccion, *cantidad);
            break;

        case 4:
            //printf("Modificar Item (No disponible por el momento)\n");
            modificarItem(*coleccion, *cantidad);
            guardarArchivo(*coleccion, *cantidad);
            break;

        case 5:
            //printf("Eliminar Item (No disponible por el momento)\n");
            eliminarItem(coleccion, cantidad);
            guardarArchivo(*coleccion, *cantidad);
            break;

        case 6:
            menuReportes(*coleccion, *cantidad);
            break;

        case 7:
            system("cls || clear");
            cicloProgreso();
            guardarArchivo(*coleccion, *cantidad);
            printf("Saliendo...\n");
            system("pause");
            break;
        
        default:
            printf("Opcion no valida. Ingrese una opcion disponible nuevamente\n");
            break;
        }
    }while(opc != 7);
}

void agregarItem(Item **coleccion, int *cantidad, int n){
    int estado;
    time_t t;
    struct tm *fecha;

    // Obtener tiempo actual
    t = time(NULL);

    // Convertir a fecha local
    fecha = localtime(&t);

    int dia = fecha->tm_mday;
    int mes = fecha->tm_mon + 1;   // tm_mon va de 0 a 11
    int year = fecha->tm_year + 1900; // tm_year cuenta desde 1900

    *coleccion = (Item*)realloc(*coleccion, (*cantidad + n) * sizeof(Item));

    if(*coleccion == NULL){
        printf("Ocurrio un error al momento de reservar el espacio de memoria");
        return;
    }

    cout << "\n╔══════════════════════════════════╗\n";
    cout << "║          AGREGAR NUEVOS          ║\n";
    cout << "╚══════════════════════════════════╝\n";

    for(int i = *cantidad; i < *cantidad + n; i++){
        if((i + 1) < 10){
            cout << "\n╔══════════════════════════════════╗\n";
            cout << "║                " << i + 1 <<"                 ║\n";
            cout << "╚══════════════════════════════════╝\n";
        }else{
            cout << "\n╔══════════════════════════════════╗\n";
            cout << "║               " << i + 1 <<"                 ║\n";
            cout << "╚══════════════════════════════════╝\n";
        }
        
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

        (*coleccion + i)->fecha.dia = dia;
        (*coleccion + i)->fecha.mes = mes;
        (*coleccion + i)->fecha.year = year;

        //id incrementa automaticamente
        (*coleccion + i)->id = i + 1;

    }

    *cantidad += n;

    cout << "\nPresione ENTER para agregar nuevos elementos...";
    getchar(); getchar();

    cicloProgreso();


}

void mostrarItems(Item *coleccion, int cantidad){
    cout << left
        << setw(5) << "ID"
        << setw(35) << "Titulo"
        << setw(15) << "Autor"
        << setw(6) << "Año"
        << setw(15) << "Genero"
        << setw(16) << "Estado"
        << endl;
    cout << string(92,'-') << endl;

    for(int i = 0; i < cantidad; i++){

        cout << left
            << setw(5) << (coleccion + i)->id
            << setw(35) << (coleccion + i)->titulo
            << setw(15) << (coleccion + i)->autor
            << setw(6) << (coleccion + i)->anio
            << setw(15) << (coleccion + i)->genero
            << setw(16) << ((coleccion + i)->estado == DISPONIBLE ? "Disponible" : "Prestado")
            << endl;
    }
    printf("\n");

    cout << "Presione ENTER para continuar...";
    getchar(); getchar();
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
    -tamano: sizeof(struct) para asegurar que se escriben bien todos los bytes
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
        printf("No hay datos previos\n\n");
        return;
    }

    //Calcular cuantos elementos hay en el archivo
    //1. Ir al final del archivo
    fseek(f, 0, SEEK_END);

    //2. Obtener el tamano en bytes con la funcion ftell()
    long tamano = ftell(f);

    //3. Calcular cantidad de Items
    *cantidad = tamano / sizeof(Item);

    //4. Regresar al inicio
    rewind(f);

    //Reservar memoria
    *coleccion = (Item*)malloc(*cantidad * sizeof(Item));
    //Verificar la creacion de memoria
    if(*coleccion == NULL){
        printf("Error al asignar memoria\n\n");
        fclose(f);
        return;
    }

    //Los parametros para leer con fread() son los mismos que para fwrite()
    fread(*coleccion, sizeof(Item), *cantidad, f);

    fclose(f);
    printf("Datos cargados correctamente\n\n");
}

int buscarItem(Item *coleccion, int cantidad, int tipoBusqueda, void *valor){
    for(int i = 0; i < cantidad; i++){

        switch(tipoBusqueda){
        //Buscar en ID
        case 1:
            if(coleccion[i].id == *(int*)valor)
                return i;
            break;

        //Buscar en Titulo
        case 2:
            if(strcmp(coleccion[i].titulo, (char*)valor) == 0)
                return i;
            break;

        //Buscar en Autor
        case 3:
            if(strcmp(coleccion[i].autor, (char*)valor) == 0)
                return i;
            break;

        //Buscar en Genero
        case 4:
            if(strcmp(coleccion[i].genero, (char*)valor) == 0)
                return i;
            break;

        //Buscar en Estado
        case 5:
            if(coleccion[i].estado == *(int*)valor)
                return i;
            break;
        }
    }

    return -1;
}

void buscarItem2(Item *coleccion, int cantidad, int tipoBusqueda, void *valor){
    
    bool encontrado = false;

    for(int i = 0; i < cantidad; i++) {
        bool match = false;

        switch(tipoBusqueda) {

        case 1: // Buscar por ID (exacta)
            match = (coleccion[i].id == *(int*)valor);
            break;

        case 2: // Buscar por titulo (parcial)
            match = (strstr(coleccion[i].titulo, (char*)valor) != NULL);
            break;

        case 3: // Buscar por autor (parcial)
            match = (strstr(coleccion[i].autor, (char*)valor) != NULL);
            break;

        case 4: // Buscar por genero (parcial)
            match = (strstr(coleccion[i].genero, (char*)valor) != NULL);
            break;

        case 5: // Buscar por estado (exacta)
            match = (coleccion[i].estado == *(int*)valor);
            break;

        }

        if(match) {
            if(!encontrado) {
                cicloProgreso();
                imprimirEncabezado();
                encontrado = true;
            }

            buzz(coleccion, i);
        }
    }

    if(!encontrado) {
        cout << "\nItem no encontrado\n";
    }
}

void menuBuscar(Item *coleccion, int cantidad){
    int opc;

    do{
        system("cls || clear");
        cout << "\n";
        cout << "╔══════════════════════════════════╗\n";
        cout << "║             BUSQUEDA             ║\n";
        cout << "╠══════════════════════════════════╣\n";
        cout << "║  1) Por ID                       ║\n";
        cout << "║  2) Por Titulo                   ║\n";
        cout << "║  3) Por Autor                    ║\n";
        cout << "║  4) Por Genero                   ║\n";
        cout << "║  5) Por Estado                   ║\n";
        cout << "║  6) Regresar al menu principal   ║\n";
        cout << "╠══════════════════════════════════╣\n";
        cout << "║  Seleccione una opción:          ║\n";
        cout << "╚══════════════════════════════════╝\n";
        cout << "➜ ";
        scanf("%d", &opc);
        cout << "\n";

        if(opc == 6){
            return;     //Regresar a menu principal
        }


        //Buscar Item por ID
        if(opc == 1){
            int id;
            printf("Ingrese ID: ");
            scanf("%d", &id);
            buscarItem2(coleccion, cantidad, 1, &id);
        }

        //Buscar Item por Titulo, Autor ó Genero
        else if(opc >= 2 && opc <= 4){
            char texto[100];
            getchar();
            printf("Ingrese texto: ");
            fgets(texto, 100, stdin);
            texto[strcspn(texto, "\n")] = '\0';

            buscarItem2(coleccion, cantidad, opc, texto);
        }

        //Buscar Item por Estado
        else if(opc == 5){
            int estado;
            printf("Estado (0=Disponible, 1=Prestado): ");
            scanf("%d", &estado);
            buscarItem2(coleccion, cantidad, 5, &estado);
        }

        //Mostrar la informacion del item encontrado
        cout << "\n\nPresione ENTER para continuar...";
        getchar(); getchar();

    }while(1);
}

void modificarItem(Item *coleccion, int cantidad){
    int id, pos;

    printf("Ingrese el ID del item a modificar: ");
    scanf("%d", &id);

    //Reutilizar la funcion buscarItem y guardar la posicion donde se encuentre el Item
    pos = buscarItem(coleccion, cantidad, 1, &id);

    system("cls || clear");

    if(pos == -1){
        printf("Item no encontrado\n");
        cout << "\nPresione ENTER para continuar...";
        getchar(); getchar();
        return;
    }
    cicloProgreso();

    //Mostrar los datos del Item encontrado
    cout << "\n╔══════════════════════════════════╗\n";
    cout << "║          ITEM ENCONTRADO         ║\n";
    cout << "╚══════════════════════════════════╝\n";

    cout << left
        << setw(35) << "Titulo"
        << setw(15) << "Autor"
        << setw(6) << "Año"
        << setw(15) << "Genero"
        << setw(16) << "Estado"
        << endl;
    cout << string(87,'-') << endl;

    cout << left
        << setw(35) << coleccion[pos].titulo
        << setw(15) << coleccion[pos].autor
        << setw(6) << coleccion[pos].anio
        << setw(15) << coleccion[pos].genero
        << setw(16) << (coleccion[pos].estado == DISPONIBLE ? "Disponible" : "Prestado")
        << endl;


    cout << "\n╔══════════════════════════════════╗\n";
    cout << "║           NUEVOS DATOS           ║\n";
    cout << "╚══════════════════════════════════╝\n";


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

    system("cls || clear");
    cicloProgreso();

    printf("Item modificado correctamente\n");

    cout << "\nPresione ENTER para continuar...";
    getchar(); getchar();
}

void eliminarItem(Item **coleccion, int *cantidad){

    int id;

    printf("Ingrese el ID del item a eliminar: ");
    scanf("%d", &id);

    int pos = buscarItem(*coleccion, *cantidad, 1, &id);

    system("cls || clear");

    if(pos == -1){
        printf("Item no encontrado\n");
        cout << "\nPresione ENTER para continuar...";
        getchar(); getchar();
        return;
    }
    cicloProgreso();

    cout << "\n╔══════════════════════════════════╗\n";
    cout << "║          ITEM ENCONTRADO         ║\n";
    cout << "╚══════════════════════════════════╝\n";

    cout << left
        << setw(5) << "ID"
        << setw(35) << "Titulo"
        << setw(15) << "Autor"
        << setw(6) << "Año"
        << setw(15) << "Genero"
        << setw(16) << "Estado"
        << endl;
    cout << string(87,'-') << endl;

    cout << left
        << setw(5) << (*coleccion)[pos].id
        << setw(35) << (*coleccion)[pos].titulo
        << setw(15) << (*coleccion)[pos].autor
        << setw(6) << (*coleccion)[pos].anio
        << setw(15) << (*coleccion)[pos].genero
        << setw(16) << ((*coleccion)[pos].estado == DISPONIBLE ? "Disponible" : "Prestado")
        << endl;

    //Confirmar la eliminacion del item
    int confirmacion;
    do{
        printf("\nSeguro que deseas eliminar este item? (0 = Cancelar | 1 = Eliminar): ");
        scanf("%d", &confirmacion);

        if(confirmacion == 0){
            printf("\nEliminacion cancelada\n");
            cout << "\nPresione ENTER para continuar...";
            getchar(); getchar();
            return;
        }else if(confirmacion == 1){
            //Desplazamos los elementos
            for(int i = pos; i < *cantidad - 1; i++){
                (*coleccion)[i] = (*coleccion)[i + 1];
            }

            //Reducimos el valor de cantidad en 1
            (*cantidad)--;

            //Reducimos memoria con realloc
            Item *temp = (Item*)realloc(*coleccion, (*cantidad) * sizeof(Item));

            //Validamos el realloc
            if(temp != NULL || *cantidad == 0){
                *coleccion = temp;
            }

            //Reasignamos los id's
            for(int i = 0; i < *cantidad; i++){
                (*coleccion)[i].id = i + 1;
            }
            cicloProgreso();

            printf("Item eliminado correctamente\n");
            cout << "\nPresione ENTER para continuar...";
            getchar(); getchar();
        }else{
            printf("Ingrese una opcion valida.\n");
            cout << "\nPresione ENTER para continuar...";
            getchar(); getchar();
        }
    }while(confirmacion != 0 && confirmacion != 1);
    
}

void buzz(Item *coleccion, int pos){
    cout << left
        << setw(5) << coleccion[pos].id
        << setw(35) << coleccion[pos].titulo
        << setw(15) << coleccion[pos].autor
        << setw(6) << coleccion[pos].anio
        << setw(15) << coleccion[pos].genero
        << setw(16) << (coleccion[pos].estado == DISPONIBLE ? "Disponible" : "Prestado")
        << endl;
}

void imprimirEncabezado(string titulo){
    const int anchoInterno = 34;

    int espacios = anchoInterno - titulo.length();
    int izq = espacios / 2;
    int der = espacios - izq;

    cout << "\n╔══════════════════════════════════╗\n";
    cout << "║"
         << string(izq, ' ')
         << titulo
         << string(der, ' ')
         << "║\n";
    cout << "╚══════════════════════════════════╝\n";

    cout << left
        << setw(5) << "ID"
        << setw(35) << "Titulo"
        << setw(15) << "Autor"
        << setw(6) << "Año"
        << setw(15) << "Genero"
        << setw(16) << "Estado"
        << endl;

    cout << string(92,'-') << endl;
}

void mostrarProgreso(double progreso) {
    const int anchoBarra = 50;

    cout << "\r[";
    
    int posicion = progreso * anchoBarra;

    for (int i = 0; i < anchoBarra; i++) {
        if (i < posicion)
            cout << "=";
        else if (i == posicion)
            cout << ">";
        else
            cout << " ";
    }

    cout << "] " << int(progreso * 100) << "%";
    cout.flush();
}

void pausa() {
    for(long i = 0; i < 4000000; i++) {
        // ciclo vacío para retraso
    }
}

void cicloProgreso(){
    int total = 100;

    for (int i = 0; i <= total; i++) {
        double progreso = (double)i / total;
        mostrarProgreso(progreso);

        pausa();
    }

    cout << "\nProceso completado exitosamente.\n";
}

void menuReportes(Item *coleccion, int cantidad){
    int opc, m, y;
    time_t t;
    struct tm *fecha;

    // Obtener tiempo actual
    t = time(NULL);

    // Convertir a fecha local
    fecha = localtime(&t);

    int mes = fecha->tm_mon + 1;   // tm_mon va de 0 a 11

    do{
        system("cls || clear");
        cout << "\n";
        cout << "╔══════════════════════════════════╗\n";
        cout << "║             REPORTES             ║\n";
        cout << "╠══════════════════════════════════╣\n";
        cout << "║  1) Mes actual                   ║\n";
        cout << "║  2) Por mes                      ║\n";
        cout << "║  3) Por año                      ║\n";
        cout << "║  4) Regresar al menu principal   ║\n";
        cout << "╠══════════════════════════════════╣\n";
        cout << "║  Seleccione una opción:          ║\n";
        cout << "╚══════════════════════════════════╝\n";
        cout << "➜ ";
        scanf("%d", &opc);
        cout << "\n";

        switch(opc){
            case 1:
                reportes(coleccion, cantidad, mes, opc);
                break;
            case 2:
                cout << "Ingrese mes (1-12): ";
                cin >> m;
                reportes(coleccion, cantidad, m, opc);
                break;
            case 3:
                cout << "Ingrese año (0000): ";
                cin >> y;
                reportes(coleccion, cantidad, y, opc);
                break;
            case 4: 
                return;
                break;
            default: 
                cout << "\nOpcion invalida...";
                break;
        }

        //Mostrar la informacion del item encontrado
        cout << "\n\nPresione ENTER para continuar...";
        getchar(); getchar();

    }while(opc != 4);
}

string nombreMes(int mes){
    string meses[] = {
        "ENERO", "FEBRERO", "MARZO", "ABRIL",
        "MAYO", "JUNIO", "JULIO", "AGOSTO",
        "SEPTIEMBRE", "OCTUBRE", "NOVIEMBRE", "DICIEMBRE"
    };

    if(mes >= 1 && mes <= 12)
        return meses[mes - 1];

    return "MES INVALIDO";
}

void reportes(Item *coleccion, int cantidad, int p, int opc){
    bool encontrado = false;
    string tituloReporte;

    time_t t;
    struct tm *fecha;
    // Obtener tiempo actual
    t = time(NULL);
    // Convertir a fecha local
    fecha = localtime(&t);
    int year = fecha->tm_year + 1900; // tm_year cuenta desde 1900

    for(int i = 0; i < cantidad; i++) {
        bool match = false;

        if(opc == 1 || opc == 2){
            match = (coleccion[i].fecha.mes == p && coleccion[i].fecha.year == year);
            tituloReporte = nombreMes(p);
        }

        if(opc == 3){
            match = (coleccion[i].fecha.year == p);
            tituloReporte = to_string(p);
        }

        if(match) {
            if(!encontrado) {
                cicloProgreso();
                imprimirEncabezado(tituloReporte);
                encontrado = true;
            }

            buzz(coleccion, i);
        }
    }

    if(!encontrado) {
        cout << "\nItem no encontrado\n";
    }
}