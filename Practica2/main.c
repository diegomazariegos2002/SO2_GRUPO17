#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "Carga_Usuario/cargaUsuario.h"  

void pausar() {
    printf("Presione Enter para continuar...");
    getchar();
    getchar();
}

void mostrarMenuReporte() {
    int opcion2;
    do {
        printf("\n<><><><><><><><> GRUPO 17 <><><><><><><><><><>");
        printf("\nReportes\n");
        printf("1. Estado de cuenta\n");
        /*
        printf("2. Carga de usuarios\n");
        printf("3. Carga de operaciones\n");
        */
        printf("4. Regresar\n");
        printf("5. Salir\n");
        printf("<><><><><><><><><><><><><><><><><><><><><><><><>\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion2);

        switch (opcion2) {
            case 1:
                //printf("Estado de cuenta\n");
                Reporte_EC();
                pausar();
                break;
                /*
            case 2:
                printf("Cargas masivas de usuarios\n");
                pausar();
                break;
            case 3:
                printf("Cargas masivas de operaciones \n");
                pausar();
                break;
                */
            case 4:
                printf("Regresando al menu principal...\n");
                break;
            case 5:
                printf("Saliendo del programa...\n");
                exit(0);
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion2 != 4);
}

void mostrarMenuCarga() {
    int opcion2;
    do {
        system("clear");
        printf("\n<><><><><><><><> GRUPO 17 <><><><><><><><><><>");
        printf("\nCargas\n");
        printf("1. Cargas masivas de usuarios\n");
        printf("2. Cargas masivas de operaciones\n");
        printf("3. Regresar\n");
        printf("4. Salir\n");
        printf("<><><><><><><><><><><><><><><><><><><><><><><><>\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion2);

        switch (opcion2) {
            case 1:
                
                char ruta[1000];
                printf("Ingrese la ruta del archivo Json\n");
                scanf("%499s", ruta);
                printf("Ruta ingresada: %s\n", ruta);
                system("clear");
                cargarUsuarios(ruta);
                //cargarUsuarios("/home/angel/Documentos/GitHub/SO2_GRUPO17./Practica2/archivos_de_entrada_(prueba)/usuarios.json");
                pausar();
                break;
            case 2:
                printf("Cargas masivas de operaciones \n");
                pausar();
                break;
            case 3:
                printf("Regresando al menu principal...\n");
                break;
            case 4:
                printf("Saliendo del programa...\n");
                exit(0);
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion2 != 3);
}

int main() {
    int opcion;
    do {
        system("clear");
        printf("\n<><><><><><><><> GRUPO 17 <><><><><><><><><><>");
        printf("\nMenu\n");
        printf("1. Cargas masivas\n");
        printf("2. Reportes\n");
        printf("3. Salir\n");
        printf("<><><><><><><><><><><><><><><><><><><><><><><><>\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                system("clear");
                mostrarMenuCarga();
                break;
            case 2:
                system("clear");
                mostrarMenuReporte();
                break;
            case 3:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 3);

    return 0;
}
