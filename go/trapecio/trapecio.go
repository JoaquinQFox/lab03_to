package trapecio

import "trapecio/funcion"

type Trapecio struct {
	Funcion   funcion.Funcion
	Inicio    float64
	Fin       float64
	Trapecios int
	Resultado float64
}

func NewTrapecio(f funcion.Funcion, inicio float64, fin float64, trapecios int) Trapecio {
	return Trapecio{
		Funcion:   f,
		Inicio:    inicio,
		Fin:       fin,
		Trapecios: trapecios,
	}
}

func (t *Trapecio) Run() {
	h := (t.Fin - t.Inicio) / float64(t.Trapecios)
	resultado := 0.0
	for i := 0; i < t.Trapecios; i++ {
		x1 := t.Inicio + float64(i)*h
		x2 := x1 + h
		area := (h / 2) *
			(t.Funcion.Evaluar(x1) + t.Funcion.Evaluar(x2))
		resultado += area
	}
	t.Resultado = resultado
}

func (t Trapecio) GetResultado() float64 {
	return t.Resultado
}

func (t Trapecio) CalcularParte(desde int, hasta int) float64 {
	h := (t.Fin - t.Inicio) / float64(t.Trapecios)
	resultado := 0.0
	for i := desde; i < hasta; i++ {
		x1 := t.Inicio + float64(i)*h
		x2 := x1 + h
		area := (h / 2) *
			(t.Funcion.Evaluar(x1) + t.Funcion.Evaluar(x2))
		resultado += area
	}
	return resultado
}
