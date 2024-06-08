#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#define LETRAS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

int main(int argc, char *argv[]) {
    // Imprimir mensaje de inicio y argumentos recibidos
    printf("son.c iniciado por father.c\n");
    printf("PID del proceso hijo: %d\n", getpid());
    if (argc > 1) {
        printf("Argumento 1: %s\n", argv[1]);
    }
    if (argc > 2) {
        printf("Argumento 2: %s\n", argv[2]);
    }

    int archivo = open("practica1.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (archivo == -1) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int opcion = rand() % 3;
        if (opcion == 0) { // open
            int archivo2 = open("practica1.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
            if (archivo2 == -1) {
                perror("Error al abrir el archivo");
                exit(EXIT_FAILURE);
            }
            printf("Se abrió el archivo\n");
            close(archivo2);
        } else if (opcion == 1) { // Write
            char texto[9];
            for (size_t i = 0; i < 8; ++i) {
                texto[i] = LETRAS[rand() % (sizeof(LETRAS) - 1)];
            }
            texto[8] = '\0';
            if (write(archivo, texto, 8) == -1) {
                perror("Error al escribir en el archivo");
                exit(EXIT_FAILURE);
            }
            printf("Escribió: %s\n", texto);
        } else if (opcion == 2) { // Read
            char texto[8 + 1];
            if (read(archivo, texto, 8) == -1) {
                perror("Error al leer del archivo");
                exit(EXIT_FAILURE);
            }
            texto[8] = '\0';
            printf("Leyó: %s\n", texto);
        }
        sleep(rand() % 3 + 1);
    }
    close(archivo);
    return 0;
}
