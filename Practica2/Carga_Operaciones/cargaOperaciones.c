#include "cargaOperaciones.h"
#include "../Carga_Usuario/cargaUsuario.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <time.h>

#define NUM_THREADS 4

Operacion *operaciones;
int num_operaciones = 0;
pthread_mutex_t lock;
Error* listaErrores = NULL;
// Variables globales para contar las operaciones
int depositos = 0;
int retiros = 0;
int transferencias = 0;
int operacionesPorHilo[NUM_THREADS] = {0};

// Funcion para obtener la fecha y hora actual en el formato del nombre del archivo
void obtenerFechaHoraArchivo(char* buffer, size_t bufferSize) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(buffer, bufferSize, "./Reportes/operaciones_%Y_%m_%d-%H_%M_%S.log", &tm);
}

// Funcion para obtener la fecha y hora actual en el formato deseado para el contenido del archivo
void obtenerFechaHoraContenido(char* buffer, size_t bufferSize) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", &tm);
}

void generarReporteOperaciones() {
    char filename[100];
    obtenerFechaHoraArchivo(filename, sizeof(filename));

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error al crear el archivo de reporte");
        return;
    }

    fprintf(file, "---------- Resumen de operaciones ----------\n");
    char fechaHora[20];
    obtenerFechaHoraContenido(fechaHora, sizeof(fechaHora));
    fprintf(file, "Fecha: %s\n\n", fechaHora);
    fprintf(file, "Operaciones realizadas:\n");
    fprintf(file, "Retiros: %d\n", retiros);
    fprintf(file, "Depositos: %d\n", depositos);
    fprintf(file, "Transferencias: %d\n", transferencias);
    fprintf(file, "Total: %d\n\n", retiros + depositos + transferencias);
    
    fprintf(file, "Operaciones por hilo:\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        fprintf(file, "Hilo #%d: %d\n", i + 1, operacionesPorHilo[i]);
    }
    fprintf(file, "Total: %d\n\n", retiros + depositos + transferencias);

    fprintf(file, "Errores:\n");
    Error* actual = listaErrores;
    while (actual != NULL) {
        fprintf(file, "Linea #%d: %s\n", actual->linea, actual->descripcion);
        actual = actual->siguiente;
    }

    fclose(file);
}

void registrarError(int linea, const char* descripcion) {
    Error* nuevoError = malloc(sizeof(Error));
    nuevoError->linea = linea;
    strcpy(nuevoError->descripcion, descripcion);
    nuevoError->siguiente = listaErrores;
    listaErrores = nuevoError;
}

void* procesar_operaciones_thread(void* arg) {
    int thread_id = *(int*)arg;
    int operaciones_por_hilo = num_operaciones / NUM_THREADS;
    int inicio = thread_id * operaciones_por_hilo;
    int fin = (thread_id == NUM_THREADS - 1) ? num_operaciones : inicio + operaciones_por_hilo;

    for (int i = inicio; i < fin; i++) {
        Operacion op = operaciones[i];
        Usuario *cuenta1 = buscarUsuario(op.cuenta1);
        Usuario *cuenta2 = (op.operacion == 3) ? buscarUsuario(op.cuenta2) : NULL;

        if (cuenta1 == NULL || (op.operacion == 3 && cuenta2 == NULL)) {
            registrarError(i + 1, "Numero de cuenta no existe");
            continue;
        }

        int resultado;
        switch (op.operacion) {
            case 1: // Deposito
                resultado = depositar(cuenta1, op.monto);
                if (resultado != 0) {
                    registrarError(i + 1, "Error en deposito");
                } else {
                    depositos++;
                    operacionesPorHilo[thread_id]++;
                }
                break;
            case 2: // Retiro
                resultado = retirar(cuenta1, op.monto);
                if (resultado != 0) {
                    registrarError(i + 1, "Saldo insuficiente");
                } else {
                    retiros++;
                    operacionesPorHilo[thread_id]++;
                }
                break;
            case 3: // Transferencia
                resultado = transferir(cuenta1, cuenta2, op.monto);
                if (resultado != 0) {
                    registrarError(i + 1, "Saldo insuficiente");
                } else {
                    transferencias++;
                    operacionesPorHilo[thread_id]++;
                }
                break;
            default:
                registrarError(i + 1, "Operacion no valida");
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
    pthread_t threads[NUM_THREADS]; // Declaracion de un array de identificadores de hilos.
    int thread_ids[NUM_THREADS]; // Declaracion de un array de identificadores de hilos.

    pthread_mutex_init(&lock, NULL); // Inicializacion del mutex.

    // Creacion de los hilos.
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i; // Asignacion del identificador al hilo.
        pthread_create(&threads[i], NULL, procesar_operaciones_thread, &thread_ids[i]); // Creacion del hilo y asignacion de la funcion 'procesar_operaciones_thread'.
    }

    // Espera a que todos los hilos terminen su ejecucion.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL); // Bloquea hasta que el hilo especificado termine.
    }

    pthread_mutex_destroy(&lock); // Destruccion del mutex.
}

void ejecutarCargaMasivaOperaciones(const char *filename) {
    cargarOperaciones(filename);
    procesarOperaciones();
    generarReporteOperaciones();
}
