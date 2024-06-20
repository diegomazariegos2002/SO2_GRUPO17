## Adicional para STAP

```
sudo apt-get update
sudo apt-get install systemtap
```

```s
sudo apt install systemtap linux-headers-$(uname -r)
```

### Verificación stap

`stap -V`

## Ejecución del monitor

En caso de error con permisos de ejecución ejecutar:

```s
chmod +x run_memory_monitor.sh
```

luego se puede ejecutar con:

```s
./run_memory_monitor.sh
```



