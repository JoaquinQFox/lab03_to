package funcion

type Funcion struct{}

func (f Funcion) Evaluar(x float64) float64 {
	return 2*x*x + 3*x + 0.5
}
