#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <cmath>
#include <iomanip>

#include "funcion/funcion.h"
#include "trapecio/trapecio.h"
#include "thread_pool/thread_pool.h"

using namespace std;

double integrar(const Funcion& funcion, double inicio, double fin, int n, ThreadPool& pool) {
    unsigned cantidadThreads = thread::hardware_concurrency();
    if (cantidadThreads == 0) {
        cantidadThreads = 4;
    }

    unsigned int threadsUsados = min(
        cantidadThreads,
        static_cast<unsigned>(n)
    );
    vector<double> resultados(threadsUsados);
    mutex mutexFinal;
    condition_variable condicionFinal;
    int pendientes = threadsUsados;
    int base = n / threadsUsados;
    int resto = n % threadsUsados;
    int desde = 0;

    for (unsigned i = 0; i < threadsUsados; i++) {
        int cantidad = base;

        if (i < static_cast<unsigned>(resto)) {
            cantidad++;
        }
        int hasta = desde + cantidad;
        pool.enqueue([&, i, desde, hasta]() {

            Trapecio trapecio(funcion, inicio, fin, n);
            resultados[i] = trapecio.calcularParte(desde, hasta);
            {
                lock_guard<mutex> lock(mutexFinal);
                pendientes--;
                if (pendientes == 0) {
                    condicionFinal.notify_one();
                }
            }
        });
        desde = hasta;
    }
    {
        unique_lock<mutex> lock(mutexFinal);

        condicionFinal.wait(lock, [&] {
            return pendientes == 0;
        });
    }
    double resultadoTotal = 0;
    for (double resultado : resultados) {
        resultadoTotal += resultado;
    }
    return resultadoTotal;
}

int main() {
    Funcion funcion;
    double inicio = 10;
    double fin = 20;
    ThreadPool pool;
    double resultadoAnterior = 0;
    cout << fixed << setprecision(5);

    for (int n = 1; n <= 1000000; n++) {
        double resultado = integrar(funcion, inicio, fin, n, pool);

        cout << "N = " << n << " | Area = " << resultado << endl;

        if (n > 1 && abs(resultado - resultadoAnterior) < 0.000001) {
            cout << "\nResultado repetido." << endl;
            cout << "N = " << n << endl;
            cout << "Area = " << resultado << endl;

            break;
        }
        resultadoAnterior = resultado;
    }
    return 0;
}