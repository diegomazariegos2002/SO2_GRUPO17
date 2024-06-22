const { Router } = require('express');
const indexController = require('../controllers/indexController');


class IndexRoutes {
    constructor() {
        this.router = Router();
        this.config();
    }

    config() {
        this.router.get('/', indexController.Inicio);
        this.router.get('/proceso', indexController.GetProceso);
        this.router.get('/llamada', indexController.GetLlamada);
        this.router.get('/top', indexController.GetTop);
    }
}

const indexRoutes = new IndexRoutes();
module.exports = indexRoutes.router;