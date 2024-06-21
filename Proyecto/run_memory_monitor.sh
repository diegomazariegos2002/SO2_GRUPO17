#!/bin/bash

# Compilar el programa en C
gcc -o memory_monitor memory_monitor.c -lmysqlclient

# Verificar si la compilación fue exitosa
if [ $? -eq 0 ]; then
    echo "Compilación exitosa. Ejecutando el programa..."
    sudo ./memory_monitor
else
    echo "Error en la compilación. No se ejecutará el programa."
fi
