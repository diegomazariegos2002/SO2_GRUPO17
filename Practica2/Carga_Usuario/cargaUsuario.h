#ifndef CARGAUSUARIO_H
#define CARGAUSUARIO_H

typedef struct Usuario {
    int no_cuenta;
    char nombre[50];
    double saldo;
    struct Usuario* siguiente;
} Usuario;

void cargarUsuarios(const char* rutaArchivo);
Usuario* buscarUsuario(int numeroCuenta);
void Reporte_EC();
int depositar(Usuario* cuenta, double monto);
int retirar(Usuario* cuenta, double monto);
int transferir(Usuario* cuenta1, Usuario* cuenta2, double monto);

extern Usuario* listaUsuarios;

#endif
