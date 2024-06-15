#ifndef CARGA_OPERACIONES_H
#define CARGA_OPERACIONES_H

typedef struct {
    int operacion;
    int cuenta1;
    int cuenta2;
    double monto;
} Operacion;

void cargarOperaciones(const char *filename);
void procesarOperaciones();
void generarReporteOperaciones();

#endif // CARGA_OPERACIONES_H
