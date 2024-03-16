#include "hello.h"

long	thread_count;

void	hello(void)
{
	#ifdef _OPENMP
		int	my_rank = omp_get_thread_num();
		int	thread_count = omp_get_num_threads();
	#else
		int	my_rank = 1;
		int	thread_count = 1;
	#endif
	printf("Greetings from thread %d of %d!\n", my_rank, thread_count);
}

int	main(int argc, char **argv)
{
	int	thread_count;
	
	(void)argc;	
	thread_count = atoi(argv[1]);

	#pragma omp parallel num_threads(thread_count)
		hello();

	return (0);
}
