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

Ejecutar un .STP manualmente

`sudo stap nameSTP.stp`

## Ejecución del monitor

En caso de error con permisos de ejecución ejecutar:

```s
chmod +x run_memory_monitor.sh
```

luego se puede ejecutar con:

```s
./run_memory_monitor.sh
```

## MySQL

### Instalar MySQL
En este proyecto se utilizo mysql de forma local, por lo que necesitaremos instalarlo.Adicional

`sudo apt update`

`sudo apt install mysql-server`

En el siguiente comando ejecutar todo con "No" y hasta el último paso "Yes" (esto por fines del proyecto como tal):

`sudo mysql_secure_installation`

`sudo systemctl status mysql.service`

*Si el servicio no esta activo* `sudo systemctl start mysql.service`

### Crear base de datos

Ingresando a mysql (Enter en password ya que no tiene root)

`sudo mysql -u root -p`

#### Ejecutar Script de generación

`SHOW DATABASES;`

`SOURCE $PATH_SCRIPT.SQL$`

`SHOW DATABASES;`

### Descargar librería C-MySQL
luego de instalar mysql en linux mint correctamente, se instalaran las librerías para conectarse a MySQL desde C:

```sh
sudo apt-get install libmysqlclient-dev
```

## Evaluar resultados

Después de compilar y ejecutar el .bin generado por el file.C entonces se puede verificar el seteado a las tablas de la base de datos mediante:

Conectarse a la MySQL:

`sudo mysql -u root -p`

```mysql
mysql > SHOW DATABASES;
mysql > USE monitor;
mysql > SHOW TABLES;
mysql > SELECT * FROM LLAMADA;
mysql > SELECT * FROM PROCESO;
```



