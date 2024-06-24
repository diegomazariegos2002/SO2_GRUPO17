const { Request, Response } = require('express');
const pool = require('../database');
const dotenv = require('dotenv');
const path = require('path');

dotenv.config({ path: path.resolve(__dirname, '..', '.env') });

class IndexController {
    async Inicio(req, res) {
        res.json({ "response": "server is running!!" });
    }

    async GetLlamada(req, res) {
        try {
            pool.query(
                'SELECT * FROM monitor.LLAMADA;',
                (error, results) => {
                    if (error) {
                        res.json({ mensaje: "Error" });
                        return;
                    }
                    if (results && results.length > 0) {
                        res.json(results);
                    } else {
                        res.json({ mensaje: "Error" });
                    }
                });
        } catch (error) {
            res.json({ mensaje: "Error" });
        }
    }

    async GetProceso(req, res) {
        try {
            pool.query(
                'SELECT * FROM monitor.PROCESO;',
                (error, results) => {
                    if (error) {
                        res.json({ mensaje: "Error" });
                        return;
                    }
                    if (results && results.length > 0) {
                        res.json(results);
                    } else {
                        res.json({ mensaje: "Error" });
                    }
                });
        } catch (error) {
            res.json({ mensaje: "Error" });
        }
    }

    async GetTop(req, res) {
        try {
            const query = `
               WITH procesos_ordenados AS (
                SELECT
                    id,
                    pid,
                    nombre,
                    size,
                    porcentaje,
                    ROW_NUMBER() OVER (PARTITION BY pid ORDER BY id DESC) AS rn
                FROM
                    monitor.PROCESO
            ),
            procesos_filtrados AS (
                SELECT
                    id,
                    pid,
                    nombre,
                    size,
                    porcentaje
                FROM
                    procesos_ordenados
                WHERE
                    rn = 1
                ORDER BY
                    size DESC
            ),
            top_9 AS (
                SELECT
                    id,
                    pid,
                    nombre,
                    size,
                    porcentaje
                FROM
                    procesos_filtrados
                LIMIT 9
            ),
            resto AS (
                SELECT
                    SUM(size) AS size_total,
                    SUM(porcentaje) AS porcentaje_total
                FROM
                    (SELECT
                        id,
                        size,
                        porcentaje
                    FROM
                        procesos_filtrados
                    LIMIT 9, 18446744073709551615) AS subquery
            )
            SELECT
                id,
                pid,
                nombre,
                size,
                porcentaje
            FROM
                top_9
            UNION ALL
            SELECT
                NULL AS id,
                NULL AS pid,
                'Otros' AS nombre,
                size_total AS size,
                porcentaje_total AS porcentaje
            FROM
                resto;

            `;
            pool.query(query, (error, results) => {
                if (error) {
                    res.json({ mensaje: "Error" });
                    return;
                }
                if (results && results.length > 0) {
                    res.json(results);
                } else {
                    res.json({ mensaje: "Error" });
                }
            });
        } catch (error) {
            res.json({ mensaje: "Error" });
        }
    }
}

module.exports = new IndexController();
