#include "hello.h"


__global__ void	hello()
{
	printf("Hello World!\n");
}

int	main()
{
	hello <<<1,10>>>();
	cudaDeviceSynchronize();
	return (0);
}
