#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <time.h>
#include <sys/stat.h>
#include "cJSON.h"
#include "cargaUsuario.h"

typedef struct {
    int cant_procesado[3];
    int total;
    char errores[150][100];
} Reporte_CU;

typedef struct {
    cJSON** usuarios;
    int inicio;
    int fin;
    int contador;
    Reporte_CU* reporte;
} DatosHilo;

Usuario* listaUsuarios = NULL;
pthread_mutex_t mutexLista = PTHREAD_MUTEX_INITIALIZER;

void ReporteCU(Reporte_CU* reporte) {
    // crear la carpeta Reportes si no existe
    struct stat st = {0};
    if (stat("Reportes", &st) == -1) {
        mkdir("Reportes", 0700);
    }

    // Obtiene la fecha y hora
    time_t ahora = time(NULL);
    struct tm* tiempoLocal = localtime(&ahora);

    // nombre del archivo
    char nombreArchivo[100];
    strftime(nombreArchivo, sizeof(nombreArchivo), "Reportes/carga_%Y_%m_%d-%H_%M_%S.log", tiempoLocal);

    // abrir el archivo
    FILE* archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        perror("No se pudo crear el archivo de reporte");
        return;
    }

    // escribir el contenido
    char fechaHora[20];
    strftime(fechaHora, sizeof(fechaHora), "%Y-%m-%d %H:%M:%S", tiempoLocal);
    fprintf(archivo, "--------- Carga de usuarios ---------\n");
    fprintf(archivo, "Fecha: %s\n\n", fechaHora);
    fprintf(archivo, "Usuarios Cargados:\n");
    int total = 0;
    for (int i = 0; i < 3; i++) {
        fprintf(archivo, "Hilo #%d: %d\n", i + 1, reporte->cant_procesado[i]);
        total += reporte->cant_procesado[i];
    }
    fprintf(archivo, "Total de usuarios procesados: %d\n\n", total);

    
    fprintf(archivo, "Errores: \n");
    fprintf(archivo, "Total de errores: %d\n", reporte->total);
    for (int i = 0; i < reporte->total; i++) {
        fprintf(archivo, "  - %s\n", reporte->errores[i]);
    }

    // Cerrar el archivo
    fclose(archivo);

    printf("Reporte carga de usuario generado con exito: %s\n", nombreArchivo);
}

void Reporte_EC() {
    // crear la carpeta Reportes si no existe
    struct stat st = {0};
    if (stat("Reportes", &st) == -1) {
        mkdir("Reportes", 0700);
    }

    const char* nombreArchivo = "Reportes/Estado_cuentas.json";
    // abre el archivo para escribir
    FILE* archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        perror("No se pudo crear el archivo JSON");
        return;
    }

    // crea un array JSON
    cJSON* arrayUsuarios = cJSON_CreateArray();
    Usuario* usuario = listaUsuarios;
    while (usuario != NULL) {
        cJSON* usuarioJSON = cJSON_CreateObject();
        cJSON_AddNumberToObject(usuarioJSON, "no_cuenta", usuario->no_cuenta);
        cJSON_AddStringToObject(usuarioJSON, "nombre", usuario->nombre);
        cJSON_AddNumberToObject(usuarioJSON, "saldo", usuario->saldo);
        cJSON_AddItemToArray(arrayUsuarios, usuarioJSON);
        usuario = usuario->siguiente;
    }

    // escribe en el archivo
    char* stringUsuarios = cJSON_Print(arrayUsuarios);
    fprintf(archivo, "%s\n", stringUsuarios);

    // libera la memoria
    cJSON_Delete(arrayUsuarios);
    free(stringUsuarios);
    fclose(archivo);

    printf("Archivo JSON generado con éxito: %s\n", nombreArchivo);
}

//Busca el usuario por su numero de cuenta
Usuario* buscarUsuario(int numeroCuenta) {
    Usuario* usuarios = listaUsuarios;
    while (usuarios != NULL) {
        if (usuarios->no_cuenta == numeroCuenta) {
            return usuarios;
        }
        usuarios = usuarios->siguiente;
    }
    return NULL;
}

void agregarError(Reporte_CU* reporte, const char* mensaje) {
    if (reporte->total < 100) {
        strncpy(reporte->errores[reporte->total], mensaje, sizeof(reporte->errores[0]) - 1);
        reporte->errores[reporte->total][sizeof(reporte->errores[0]) - 1] = '\0';
        reporte->total++;
    }
}

void* procesarUsuarios(void* arg) {
    DatosHilo* datosHilo = (DatosHilo*)arg;
    cJSON** usuarios = datosHilo->usuarios;
    Reporte_CU* reporte = datosHilo->reporte;

    // ciclo para procesar los usuarios de este hilo
    for (int i = datosHilo->inicio; i < datosHilo->fin; i++) {
        cJSON* item = usuarios[i];
        cJSON* no_cuenta = cJSON_GetObjectItem(item, "no_cuenta");
        cJSON* nombre = cJSON_GetObjectItem(item, "nombre");
        cJSON* saldo = cJSON_GetObjectItem(item, "saldo");

        // valida los tipos de datos
        if (!cJSON_IsNumber(no_cuenta) || !cJSON_IsString(nombre) || !cJSON_IsNumber(saldo)) {
            char mensaje[100];
            snprintf(mensaje, sizeof(mensaje), "Error en la cuenta %d (índice %d): ", 
                     cJSON_IsNumber(no_cuenta) ? no_cuenta->valueint : -1, i);
            if (!cJSON_IsNumber(no_cuenta)) {
                strcat(mensaje, "Campo 'no_cuenta' no es un número. ");
            }
            if (!cJSON_IsString(nombre)) {
                strcat(mensaje, "Campo 'nombre' no es una cadena. ");
            }
            if (!cJSON_IsNumber(saldo)) {
                strcat(mensaje, "Campo 'saldo' no es un número. ");
            }
            pthread_mutex_lock(&mutexLista);
            agregarError(reporte, mensaje);
            pthread_mutex_unlock(&mutexLista);
            continue;
        }

        // valida el número de cuenta sea único
        int numeroCuenta = no_cuenta->valueint;
        pthread_mutex_lock(&mutexLista);
        if (buscarUsuario(numeroCuenta) != NULL) {
            char mensaje[100];
            snprintf(mensaje, sizeof(mensaje), "Error en la cuenta %d (índice %d): Número de cuenta duplicado", no_cuenta->valueint, i);
            agregarError(reporte, mensaje);
            pthread_mutex_unlock(&mutexLista);
            continue;
        }
        pthread_mutex_unlock(&mutexLista);

        // valida el saldo
        double saldoValor = saldo->valuedouble;
        if (saldoValor < 0) {
            char mensaje[100];
            snprintf(mensaje, sizeof(mensaje), "Error en la cuenta %d (índice %d): Saldo negativo", no_cuenta->valueint, i);
            pthread_mutex_lock(&mutexLista);
            agregarError(reporte, mensaje);
            pthread_mutex_unlock(&mutexLista);
            continue;
        }

        // crear una estructura nueva de usuario
        Usuario* usuario = (Usuario*)malloc(sizeof(Usuario));
        usuario->no_cuenta = no_cuenta->valueint;
        strcpy(usuario->nombre, nombre->valuestring);
        usuario->saldo = saldoValor;
        usuario->siguiente = NULL;

        // procesar el usuario
        pthread_mutex_lock(&mutexLista);
        usuario->siguiente = listaUsuarios;
        listaUsuarios = usuario;
        pthread_mutex_unlock(&mutexLista);

        // incrementar el contador
        datosHilo->contador++;
    }

    return NULL;
}

void cargarUsuarios(const char* rutaArchivo) {
    // abrir el archivo
    FILE* archivo = fopen(rutaArchivo, "r");
    if (!archivo) {
        perror("No se pudo abrir el archivo");
        return;
    }

    // leer el archivo completo
    fseek(archivo, 0, SEEK_END);
    long longitudArchivo = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);
    char* contenidoArchivo = (char*)malloc(longitudArchivo + 1);
    fread(contenidoArchivo, 1, longitudArchivo, archivo);
    fclose(archivo);
    contenidoArchivo[longitudArchivo] = '\0';

    // convierte el contenido a JSON
    cJSON* contenidoJson = cJSON_Parse(contenidoArchivo);
    if (!contenidoJson) {
        printf("Error al parsear el JSON\n");
        free(contenidoArchivo);
        return;
    }

    // obtiene la cantidad de usuarios
    int cantidadUsuarios = cJSON_GetArraySize(contenidoJson);

    // copiar los usuarios a un arreglo de cJSON*
    cJSON** usuarios = (cJSON**)malloc((cantidadUsuarios + 1) * sizeof(cJSON*));
    for (int i = 0; i < cantidadUsuarios; i++) {
        usuarios[i] = cJSON_GetArrayItem(contenidoJson, i);
    }
    usuarios[cantidadUsuarios] = NULL;

    // crea el reporte para almacenar errores
    Reporte_CU reporte;
    reporte.total = 0;
    memset(reporte.cant_procesado, 0, sizeof(reporte.cant_procesado));
    memset(reporte.errores, 0, sizeof(reporte.errores));

    // Crea hilos y procesa los usuarios
    pthread_t hilos[3];
    DatosHilo datosHilos[3];

    int usuariosPorHilo = cantidadUsuarios / 3;
    int indiceUsuario = 0;

    for (int i = 0; i < 3; i++) {
        datosHilos[i].usuarios = usuarios;
        datosHilos[i].inicio = indiceUsuario;
        datosHilos[i].fin = indiceUsuario + usuariosPorHilo;
        datosHilos[i].contador = 0;
        datosHilos[i].reporte = &reporte;
        if (i == 2) {
            datosHilos[i].fin = cantidadUsuarios;
        }
        pthread_create(&hilos[i], NULL, procesarUsuarios, &datosHilos[i]);
        indiceUsuario = datosHilos[i].fin;
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(hilos[i], NULL);
        reporte.cant_procesado[i] = datosHilos[i].contador;
        printf("Hilo %d procesó %d usuarios.\n", i, datosHilos[i].contador);
    }

    // realiza el reporte de errores
    printf("\nReporte de Errores:\n");
    printf("Total de errores encontrados: %d\n", reporte.total);
    for (int i = 0; i < reporte.total; i++) {
        printf("Error %d: %s\n", i + 1, reporte.errores[i]);
    }
    
    //crea el reporte CU
     ReporteCU(&reporte);

    //libera memoria
    cJSON_Delete(contenidoJson);
    free(contenidoArchivo);
    free(usuarios);
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