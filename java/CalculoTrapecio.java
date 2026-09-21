public class CalculoTrapecio {

    private Funcion funcion;

    public CalculoTrapecio(Funcion funcion) {
        this.funcion = funcion;
    }

    public double calcular(double a, double b, int n) {

        double h = (b - a) / n;

        double suma = (funcion.evaluar(a) + funcion.evaluar(b)) / 2;

        for (int i = 1; i < n; i++) {
            double x = a + i * h;
            suma += funcion.evaluar(x);
        }

        return h * suma;
    }
}