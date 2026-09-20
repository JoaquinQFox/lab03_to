package main

import (
	"fmt"
	"runtime"
	"sync"

	"trapecio/funcion"
	"trapecio/threadpool"
	"trapecio/trapecio"
)

func integrar(f funcion.Funcion, inicio, fin float64, n int, pool *threadpool.ThreadPool) float64 {
	workers := min(runtime.NumCPU(), n)
	resultados := make([]float64, workers)
	var wg sync.WaitGroup
	wg.Add(workers)

	base, resto, desde := n/workers, n%workers, 0

	for i := 0; i < workers; i++ {
		cantidad := base
		if i < resto {
			cantidad++
		}
		hasta := desde + cantidad
		indice, inicioParte, finParte := i, desde, hasta

		pool.Enqueue(func() {
			t := trapecio.NewTrapecio(f, inicio, fin, n)
			resultados[indice] = t.CalcularParte(inicioParte, finParte)
			wg.Done()
		})

		desde = hasta
	}

	wg.Wait()

	total := 0.0
	for _, resultado := range resultados {
		total += resultado
	}
	return total
}

func main() {
	f := funcion.Funcion{}
	inicio, fin := 10.0, 20.0
	workers := runtime.NumCPU()
	pool := threadpool.NewThreadPool(workers)

	fmt.Printf("Usando %d workers\n\n", workers)

	anterior := 0.0

	for n := 1; n <= 1000000; n++ {
		resultado := integrar(f, inicio, fin, n, pool)

		fmt.Printf("N = %d | Area = %.10f\n", n, resultado)

		if n > 1 && resultado == anterior {
			fmt.Printf("\nResultado repetido aproximado en N = %d -> Area = %.10f\n", n, resultado)
			break
		}

		anterior = resultado
	}
}
