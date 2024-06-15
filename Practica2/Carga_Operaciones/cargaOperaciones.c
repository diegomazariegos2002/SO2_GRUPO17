#include "cargaOperaciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "Carga_Usuario/cargaUsuario.h"

#define NUM_THREADS 4

Operacion *operaciones;
int num_operaciones = 0;
pthread_mutex_t lock;

void* procesar_operaciones_thread(void* arg) {
    int thread_id = *(int*)arg;
    int operaciones_por_hilo = num_operaciones / NUM_THREADS;
    int inicio = thread_id * operaciones_por_hilo;
    int fin = (thread_id == NUM_THREADS - 1) ? num_operaciones : inicio + operaciones_por_hilo;

    for (int i = inicio; i < fin; i++) {
        Operacion op = operaciones[i];
        Cuenta *cuenta1 = buscar_cuenta(op.cuenta1);
        Cuenta *cuenta2 = (op.operacion == 3) ? buscar_cuenta(op.cuenta2) : NULL;

        if (cuenta1 == NULL || (op.operacion == 3 && cuenta2 == NULL)) {
            // Manejar errores de cuenta no encontrada
            continue;
        }

        switch (op.operacion) {
            case 1: // Deposito
                depositar(cuenta1, op.monto);
                break;
            case 2: // Retiro
                if (cuenta1->saldo >= op.monto) {
                    retirar(cuenta1, op.monto);
                }
                break;
            case 3: // Transferencia
                if (cuenta1->saldo >= op.monto) {
                    transferir(cuenta1, cuenta2, op.monto);
                }
                break;
            default:
                // Manejar errores de operación desconocida
                break;
        }
    }

    return NULL;
}

void cargarOperaciones(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir el archivo de operaciones");
        return;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = malloc(length + 1);
    fread(data, 1, length, file);
    fclose(file);
    data[length] = '\0';

    cJSON *root = cJSON_Parse(data);
    if (!root) {
        fprintf(stderr, "Error al parsear JSON: %s\n", cJSON_GetErrorPtr());
        free(data);
        return;
    }

    num_operaciones = cJSON_GetArraySize(root);
    operaciones = malloc(num_operaciones * sizeof(Operacion));

    for (int i = 0; i < num_operaciones; i++) {
        cJSON *item = cJSON_GetArrayItem(root, i);
        operaciones[i].operacion = cJSON_GetObjectItem(item, "operacion")->valueint;
        operaciones[i].cuenta1 = cJSON_GetObjectItem(item, "cuenta1")->valueint;
        operaciones[i].cuenta2 = cJSON_GetObjectItem(item, "cuenta2")->valueint;
        operaciones[i].monto = cJSON_GetObjectItem(item, "monto")->valuedouble;

        // Imprimir los datos cargados
        printf("Operacion %d: cuenta1 = %d, cuenta2 = %d, monto = %.2f\n", 
               operaciones[i].operacion, 
               operaciones[i].cuenta1, 
               operaciones[i].cuenta2, 
               operaciones[i].monto);
    }

    cJSON_Delete(root);
    free(data);
}

void procesarOperaciones() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, procesar_operaciones_thread, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
}

void generarReporteOperaciones() {
    FILE *file = fopen("Reportes/carga_reporte.log", "w");
    if (!file) {
        perror("Error al crear el archivo de reporte");
        return;
    }

    // Escribir detalles de las operaciones realizadas y errores
    fprintf(file, "Reporte de operaciones...\n");

    fclose(file);
}
