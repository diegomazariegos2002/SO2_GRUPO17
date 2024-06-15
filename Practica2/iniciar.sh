#!/bin/bash

gcc -o main main.c Carga_Usuario/cargaUsuario.c -lpthread -lcjson -I/usr/include/cjson

# Verificar si la compilación fue exitosa
if [ $? -eq 0 ]; then
    echo "Compilación exitosa. Ejecutando el programa..."
    ./main
else
    echo "Error en la compilación. No se ejecutará el programa."
fi
