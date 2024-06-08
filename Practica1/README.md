# Sistemas Operativos 2
# Practica 1

## Instalaciones previas

### Linux Mint build-essential
https://community.linuxmint.com/software/view/build-essential

![alt text](images/image.png)

### Linux Mint gdb
https://community.linuxmint.com/software/view/gdb

![alt text](images/image2.png)


## Adicional para STAP

```
sudo apt-get update
sudo apt-get install systemtap
```

### Verificación stap

`stap -V`

## Proceso para ejecutar

### Crear bin hijo

### Crear bin padre

`gcc father.c -o father`

### Ejecutar stp
`sudo stap monitor.stp`

### Ejecutar padre (carpeta raíz)
`./father`