-- Eliminar la base de datos si existe
DROP DATABASE IF EXISTS monitor;

-- Crear la base de datos
CREATE DATABASE IF NOT EXISTS monitor;

-- Seleccionar la base de datos para usar
USE monitor;

-- Crear la tabla LLAMADA
CREATE TABLE LLAMADA (
    id INT AUTO_INCREMENT PRIMARY KEY,
    pid INT NOT NULL,
    nombre_proceso VARCHAR(255) NOT NULL,
    llamada VARCHAR(255) NOT NULL,
    size DOUBLE NOT NULL,
    fecha DATE NOT NULL
);

-- Crear la tabla PROCESO
CREATE TABLE PROCESO (
    id INT AUTO_INCREMENT PRIMARY KEY,
    pid INT NOT NULL,
    nombre VARCHAR(255) NOT NULL,
    size INT NOT NULL COMMENT 'Tamaño en MB',
    porcentaje DOUBLE NOT NULL
);
