#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "Carga_Usuario/cargaUsuario.h"
#include "Carga_Operaciones/cargaOperaciones.h"

void pausar() {
    printf("Presione Enter para continuar...");
    getchar();
    getchar();
}

// Funciones para operaciones individuales
void deposito() {
    int numeroCuenta;
    double monto;
    printf("Ingrese el numero de cuenta: ");
    scanf("%d", &numeroCuenta);
    printf("Ingrese el monto a depositar: ");
    scanf("%lf", &monto);

    Usuario* cuenta = buscarUsuario(numeroCuenta);
    if (cuenta == NULL) {
        printf("Numero de cuenta no existe.\n");
        return;
    }

    depositar(cuenta, monto);
    printf("Deposito realizado. Nuevo saldo: %.2f\n", cuenta->saldo);
}

void retiro() {
    int numeroCuenta;
    double monto;
    printf("Ingrese el numero de cuenta: ");
    scanf("%d", &numeroCuenta);
    printf("Ingrese el monto a retirar: ");
    scanf("%lf", &monto);

    Usuario* cuenta = buscarUsuario(numeroCuenta);
    if (cuenta == NULL) {
        printf("Numero de cuenta no existe.\n");
        return;
    }

    if (retirar(cuenta, monto) == 0) {
        printf("Retiro realizado. Nuevo saldo: %.2f\n", cuenta->saldo);
    } else {
        printf("Saldo insuficiente.\n");
    }
}

void transferencia() {
    int numeroCuentaOrigen, numeroCuentaDestino;
    double monto;
    printf("Ingrese el numero de cuenta de origen: ");
    scanf("%d", &numeroCuentaOrigen);
    printf("Ingrese el numero de cuenta de destino: ");
    scanf("%d", &numeroCuentaDestino);
    printf("Ingrese el monto a transferir: ");
    scanf("%lf", &monto);

    Usuario* cuentaOrigen = buscarUsuario(numeroCuentaOrigen);
    Usuario* cuentaDestino = buscarUsuario(numeroCuentaDestino);
    if (cuentaOrigen == NULL || cuentaDestino == NULL) {
        printf("Numero de cuenta no existe.\n");
        return;
    }

    if (transferir(cuentaOrigen, cuentaDestino, monto) == 0) {
        printf("Transferencia realizada. Nuevo saldo cuenta origen: %.2f\n", cuentaOrigen->saldo);
        printf("Nuevo saldo cuenta destino: %.2f\n", cuentaDestino->saldo);
    } else {
        printf("Saldo insuficiente.\n");
    }
}

void consultarCuenta() {
    int numeroCuenta;
    printf("Ingrese el numero de cuenta: ");
    scanf("%d", &numeroCuenta);

    Usuario* cuenta = buscarUsuario(numeroCuenta);
    if (cuenta == NULL) {
        printf("Numero de cuenta no existe.\n");
    } else {
        printf("Numero de cuenta: %d\n", cuenta->no_cuenta);
        printf("Nombre: %s\n", cuenta->nombre);
        printf("Saldo: %.2f\n", cuenta->saldo);
    }
}

void mostrarMenuOperacionesIndividuales() {
    int opcion;
    do {
        system("clear");
        printf("\n<><><><><><><><> GRUPO 17 <><><><><><><><><><>");
        printf("\nOperaciones Individuales\n");
        printf("1. Deposito\n");
        printf("2. Retiro\n");
        printf("3. Transferencia\n");
        printf("4. Consultar cuenta\n");
        printf("5. Regresar\n");
        printf("6. Salir\n");
        printf("<><><><><><><><><><><><><><><><><><><><><><><><>\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                deposito();
                pausar();
                break;
            case 2:
                retiro();
                pausar();
                break;
            case 3:
                transferencia();
                pausar();
                break;
            case 4:
                consultarCuenta();
                pausar();
                break;
            case 5:
                printf("Regresando al menu principal...\n");
                break;
            case 6:
                printf("Saliendo del programa...\n");
                exit(0);
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 5);
}

void mostrarMenuReporte() {
    int opcion2;
    do {
        printf("\n<><><><><><><><> GRUPO 17 <><><><><><><><><><>");
        printf("\nReportes\n");
        printf("1. Estado de cuenta\n");
        printf("4. Regresar\n");
        printf("5. Salir\n");
        printf("<><><><><><><><><><><><><><><><><><><><><><><><>\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion2);

        switch (opcion2) {
            case 1:
                Reporte_EC();
                pausar();
                break;
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
                char ruta_usuarios[1000];
                printf("Ingrese la ruta del archivo Json\n");
                scanf("%499s", ruta_usuarios);
                printf("Ruta ingresada: %s\n", ruta_usuarios);
                system("clear");
                cargarUsuarios(ruta_usuarios);
                pausar();
                break;
            case 2:
                char ruta_operaciones[1000];
                printf("Ingrese la ruta del archivo Json\n");
                scanf("%499s", ruta_operaciones);
                printf("Ruta ingresada: %s\n", ruta_operaciones);
                system("clear");
                ejecutarCargaMasivaOperaciones(ruta_operaciones);
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
        printf("3. Operaciones Individuales\n");
        printf("4. Salir\n");
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
                system("clear");
                mostrarMenuOperacionesIndividuales();
                break;
            case 4:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 4);

    return 0;
}
