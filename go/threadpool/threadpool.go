package threadpool

type Task func()

type ThreadPool struct {
	tasks chan Task
}

func NewThreadPool(numWorkers int) *ThreadPool {
	pool := &ThreadPool{
		tasks: make(chan Task),
	}

	for i := 0; i < numWorkers; i++ {
		go func() {
			for task := range pool.tasks {
				task()
			}
		}()
	}
	return pool
}

func (p *ThreadPool) Enqueue(task Task) {
	p.tasks <- task
}
