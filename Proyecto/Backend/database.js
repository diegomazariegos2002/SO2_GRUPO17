const mysql = require('mysql2');
const dotenv = require('dotenv');
const path = require('path');

const envPath = path.resolve(__dirname, '..', '.env');
dotenv.config({ path: envPath });

const pool = mysql.createPool({
  host: process.env.HOST,
  user: 'root',
  password: process.env.PASSWORD,
  database: process.env.DATABASE,
});

pool.getConnection((err, connection) => {
  if (err) {
    console.error('Error al conectar a la base de datos:', err.message);
    return;
  }
  console.log('Conexión a la base de datos establecida correctamente.');
});

module.exports = pool;
