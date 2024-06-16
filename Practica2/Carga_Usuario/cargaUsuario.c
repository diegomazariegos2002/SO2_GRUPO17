#include "cargaUsuario.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

Usuario* listaUsuarios = NULL;

void cargarUsuarios(const char* rutaArchivo) {
    FILE* archivo = fopen(rutaArchivo, "r");
    if (!archivo) {
        perror("Error al abrir el archivo de usuarios");
        return;
    }

    fseek(archivo, 0, SEEK_END);
    long longitud = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);
    char* datos = malloc(longitud + 1);
    fread(datos, 1, longitud, archivo);
    fclose(archivo);
    datos[longitud] = '\0';

    cJSON* root = cJSON_Parse(datos);
    if (!root) {
        fprintf(stderr, "Error al parsear JSON: %s\n", cJSON_GetErrorPtr());
        free(datos);
        return;
    }

    cJSON* usuarioJSON = NULL;
    cJSON_ArrayForEach(usuarioJSON, root) {
        Usuario* nuevoUsuario = malloc(sizeof(Usuario));
        nuevoUsuario->no_cuenta = cJSON_GetObjectItem(usuarioJSON, "no_cuenta")->valueint;
        strcpy(nuevoUsuario->nombre, cJSON_GetObjectItem(usuarioJSON, "nombre")->valuestring);
        nuevoUsuario->saldo = cJSON_GetObjectItem(usuarioJSON, "saldo")->valuedouble;
        nuevoUsuario->siguiente = listaUsuarios;
        listaUsuarios = nuevoUsuario;
    }

    cJSON_Delete(root);
    free(datos);
}

Usuario* buscarUsuario(int numeroCuenta) {
    Usuario* actual = listaUsuarios;
    while (actual != NULL) {
        if (actual->no_cuenta == numeroCuenta) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

int depositar(Usuario* cuenta, double monto) {
    cuenta->saldo += monto;
    return 0; // Exito
}

int retirar(Usuario* cuenta, double monto) {
    if (cuenta->saldo >= monto) {
        cuenta->saldo -= monto;
        return 0; // Exito
    } else {
        return -1; // Error de saldo insuficiente
    }
}

int transferir(Usuario* cuenta1, Usuario* cuenta2, double monto) {
    if (cuenta1->saldo >= monto) {
        cuenta1->saldo -= monto;
        cuenta2->saldo += monto;
        return 0; // Exito
    } else {
        return -1; // Error de saldo insuficiente
    }
}

void Reporte_EC() {
    // Implementacion del reporte de estado de cuenta
    Usuario* actual = listaUsuarios;
    while (actual != NULL) {
        printf("Cuenta: %d, Nombre: %s, Saldo: %.2f\n", actual->no_cuenta, actual->nombre, actual->saldo);
        actual = actual->siguiente;
    }
}
