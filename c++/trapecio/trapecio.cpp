#include "trapecio.h"

Trapecio::Trapecio(const Funcion &funcion, double inicio, double fin, int trapecios) :
    funcion(funcion), inicio(inicio), fin(fin), trapecios(trapecios) {}

void Trapecio::run() {
    double h = (fin - inicio) / trapecios;
    double resultado = 0;
    
    for (int i = 0; i < trapecios; i++) {
        double x1 = inicio + i * h;
        double x2 = x1 + h;

        double area = (h / 2) *
            (funcion.evaluar(x1) + funcion.evaluar(x2));

        resultado += area;
    }

    this->resultado = resultado;
}

double Trapecio::getResultado() {
    return this->resultado;
}

double Trapecio::calcularParte(int desde, int hasta) {
    double h = (fin - inicio) / trapecios;
    double resultado = 0;

    for (int i = desde; i < hasta; i++) {
        double x1 = inicio + i * h;
        double x2 = x1 + h;
        double area = (h / 2) *
            (funcion.evaluar(x1) + funcion.evaluar(x2));
        resultado += area;
    }
    return resultado;
}
