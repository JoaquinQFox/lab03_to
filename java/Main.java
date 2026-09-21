import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class Main {

    public static void main(String[] args) throws Exception {

        double a = 2;
        double b = 20;

        int N = 10000;
        double tolerancia = 0.000001;

        Funcion funcion = new Funcion();
        CalculoTrapecio calculo = new CalculoTrapecio(funcion);

        System.out.println("METODO DEL TRAPECIO");
        System.out.println("-------------------");
        System.out.println("Funcion: f(x) = 2x^2 + 3x + 0.5");
        System.out.println("Intervalo: [" + a + ", " + b + "]");
        System.out.println();

        // -----------------------------------------
        // CALCULO DE 1 HASTA N TRAPECIOS
        // -----------------------------------------

        double anterior = 0;
        double actual = 0;
        int nEncontrado = N;

        for (int n = 1; n <= N; n++) {

            actual = calculo.calcular(a, b, n);

            if (n > 1) {

                double diferencia = Math.abs(actual - anterior);

                if (diferencia < tolerancia) {

                    nEncontrado = n;

                    System.out.println("Resultado sin cambio significativo.");
                    System.out.println("Trapecios utilizados: " + n);
                    System.out.println("Area aproximada: " + actual);
                    System.out.println("Diferencia: " + diferencia);

                    break;
                }
            }

            anterior = actual;
        }

        // -----------------------------------------
        // THREADS CLASICOS
        // -----------------------------------------

        System.out.println();
        System.out.println("CALCULO PARALELO CON THREADS");
        System.out.println("----------------------------");

        int n = nEncontrado;

        int n1 = n / 2;
        int n2 = n - n1;

        double mitad = (a + b) / 2;

        double[] resultados = new double[2];

        Thread thread1 = new Thread(() -> {
            resultados[0] = calculo.calcular(
                    a,
                    mitad,
                    n1
            );
        });

        Thread thread2 = new Thread(() -> {
            resultados[1] = calculo.calcular(
                    mitad,
                    b,
                    n2
            );
        });

        long inicioThreads = System.nanoTime();

        thread1.start();
        thread2.start();

        thread1.join();
        thread2.join();

        double areaThreads = resultados[0] + resultados[1];

        long finThreads = System.nanoTime();

        System.out.println("Threads utilizados: 2");
        System.out.println("Area aproximada: " + areaThreads);
        System.out.println("Tiempo: "
                + (finThreads - inicioThreads) / 1_000_000.0
                + " ms");

        // -----------------------------------------
        // THREAD POOL
        // -----------------------------------------

        System.out.println();
        System.out.println("CALCULO CON THREAD POOL");
        System.out.println("-----------------------");

        ExecutorService pool =
                Executors.newFixedThreadPool(2);

        long inicioPool = System.nanoTime();

        Future<Double> resultado1 = pool.submit(() ->
                calculo.calcular(
                        a,
                        mitad,
                        n1
                )
        );

        Future<Double> resultado2 = pool.submit(() ->
                calculo.calcular(
                        mitad,
                        b,
                        n2
                )
        );

        double areaPool =
                resultado1.get() + resultado2.get();

        long finPool = System.nanoTime();

        pool.shutdown();

        System.out.println("Threads del Pool: 2");
        System.out.println("Area aproximada: " + areaPool);
        System.out.println("Tiempo: "
                + (finPool - inicioPool) / 1_000_000.0
                + " ms");
    }
}