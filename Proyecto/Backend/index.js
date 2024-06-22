const express = require('express');
const cors = require('cors');

const indexRoutes = require('./routes/indexRoutes.js');

class Server {

    constructor() {
        this.app = express();
        this.config();
        this.routes();
    }

    config() {
        this.app.set('port', process.env.PORT || 5000);
        this.app.use(cors());
    }

    routes() {
        this.app.use('/', indexRoutes);
    }

    start() {
        this.app.listen(this.app.get('port'), () => {
            console.log('Servidor en el puerto', this.app.get('port'));
        });
    }
}

const server = new Server();
server.start();