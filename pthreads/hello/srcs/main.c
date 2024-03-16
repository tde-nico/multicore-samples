#include "hello.h"

long	thread_count;

void	*hello(void *rank)
{
	long my_rank = (long) rank;

	printf("Greetings from thread %ld of %ld!\n", my_rank, thread_count);

	return (NULL);
}

int	main(int argc, char **argv)
{
	long		thread;
	pthread_t	*thread_handles;

	if (argc < 2)
		thread_count = 2;
	else
		thread_count = strtol(argv[1], NULL, 10);

	thread_handles = malloc(thread_count * sizeof(pthread_t));
	
	for (thread = 0; thread < thread_count; ++thread)
		pthread_create(&thread_handles[thread], NULL, hello, (void *)thread);

	printf("Hello from main thread\n");

	for (thread = 0; thread < thread_count; ++thread)
		pthread_join(thread_handles[thread], NULL);
	
	free(thread_handles);

	return (0);
}
