#ifndef TRAPECIO_H
#define TRAPECIO_H

#include "../funcion/funcion.h"

class Trapecio {
    private:
        Funcion funcion;
        double inicio;
        double fin;
        int trapecios;
        double resultado;

    public:
        Trapecio(const Funcion &funcion, double inicio, double fin, int trapecios);

        void run();

        double calcularParte(int desde, int hasta);

        double getResultado();
};

#endif