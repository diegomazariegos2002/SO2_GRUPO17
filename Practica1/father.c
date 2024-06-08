#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>

void manejarSIGINT(int signum) {
    FILE *fp;
    char line[256];
    int count_read = 0, count_write = 0, count_open = 0;

    // Abrir el archivo syscalls.log para lectura
    fp = fopen("syscalls.log", "r");
    if (fp == NULL) {
        perror("Error al abrir el archivo syscalls.log");
    }

    // Leer el archivo línea por línea
    while (fgets(line, sizeof(line), fp)) {
        // Buscar la palabra "read", "open" (sirve para "open" o "openat") y "write" en cada línea
        if (strstr(line, "read") != NULL) {
            count_read++;
        }
        if (strstr(line, "write") != NULL) {
            count_write++;
        }
        if (strstr(line, "open") != NULL) {
            count_open++;
        }
    }

    // Cerrar el archivo
    fclose(fp);

    // Imprimir los resultados
    printf("Total de llamadas al sistema: %d\n", (count_read + count_write + count_open));
    printf("Read: %d, Write: %d, Open: %d\n", count_read, count_write, count_open);
    wait(NULL);
    wait(NULL);

    exit(signum); // Salir del programa con el código de la señal
}

int main() {
    // Configurar el manejador de la señal SIGINT
    signal(SIGINT, manejarSIGINT);

    pid_t proceso_hijo = fork(); // Se realiza el fork

    if (proceso_hijo == -1) {
        perror("fork");
        exit(1);
    }

    if (proceso_hijo == 0) {
        // Este bloque se ejecuta en el proceso hijo
        char *arg_Ptr[4];
        arg_Ptr[0] = "son.bin";
        arg_Ptr[1] = " Hola! ";
        arg_Ptr[2] = " Soy el proceso hijo! ";
        arg_Ptr[3] = NULL; // El ultimo indice de argv siempre debe de ser NULL

        execv("/home/diegomazariegos/Documentos/proyectos/SO2_GRUPO17/Practica1/son.bin", arg_Ptr);
        // Si execv falla
        perror("execv");
        exit(1);
    } else {
        pid_t proceso_hijo2 = fork();
        if (proceso_hijo2 == 0) {
            sleep(1);
            char *arg_Ptr[4];
            arg_Ptr[0] = "son.bin";
            arg_Ptr[1] = " Hola! ";
            arg_Ptr[2] = " Soy el proceso hijo2! ";
            arg_Ptr[3] = NULL; // El ultimo indice de argv siempre debe de ser NULL

            execv("/home/diegomazariegos/Documentos/proyectos/SO2_GRUPO17/Practica1/son.bin", arg_Ptr);
            // Si execv falla
            perror("execv");
            exit(1);
        } else {
            char command[100];

            // Se espera a que el proceso hijo termine
            sprintf(command, "%s %d %d %s", "sudo stap monitor.stp ", proceso_hijo, proceso_hijo2, " > syscalls.log");
            system(command);

            wait(NULL);
            wait(NULL);
        }
    }

    return 0;
}
