#ifndef CARGAOPERACIONES_H
#define CARGAOPERACIONES_H

typedef struct {
    int operacion;
    int cuenta1;
    int cuenta2;
    double monto;
} Operacion;

typedef struct Error {
    int linea;
    char descripcion[100];
    struct Error* siguiente;
} Error;

void cargarOperaciones(const char *filename);
void procesarOperaciones();
void generarReporteOperaciones();
void ejecutarCargaMasivaOperaciones(const char *filename);

void registrarError(int linea, const char* descripcion);

extern Error* listaErrores;

#endif // CARGAOPERACIONES_H
