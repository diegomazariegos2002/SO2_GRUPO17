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
            pool.query(
                `SELECT pid, nombre, size, porcentaje
                    FROM (
                        SELECT pid, nombre, size, porcentaje
                        FROM monitor.PROCESO
                        ORDER BY size DESC
                        LIMIT 9
                    ) AS Top9

                    UNION ALL

                    SELECT 
                        NULL AS pid,
                        'Otros' AS nombre,
                        SUM(size) AS size,
                        SUM(porcentaje) AS porcentaje
                    FROM (
                        SELECT size, porcentaje
                        FROM monitor.PROCESO
                        ORDER BY size DESC
                        LIMIT 9, 18446744073709551615
                    ) AS Otros;
                    `,
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
}

module.exports = new IndexController();
