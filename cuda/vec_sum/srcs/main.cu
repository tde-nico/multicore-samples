#include "hello.h"

#define N 1000

__global__ void	sum(int *v1_d, int *v2_d, int *v3_d, int n)
{
	int id = threadIdx.x + blockIdx.x * blockDim.x;
	if (id < n)
		v3_d[id] = v1_d[id] + v2_d[id];
}

int	main(void)
{
	int	v1_h[N];
	int	v2_h[N];
	int	v3_h[N];

	for (int i = 0; i < N; ++i)
	{
		v1_h[i] = i + 1;
		v2_h[i] = -i;
	}

	int	*v1_d;
	int	*v2_d;
	int	*v3_d;

	cudaMalloc((void **)&v1_d, N * sizeof(int));
	cudaMalloc((void **)&v2_d, N * sizeof(int));
	cudaMalloc((void **)&v3_d, N * sizeof(int));

	cudaMemcpy(v1_d, v1_h, N * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(v2_d, v2_h, N * sizeof(int), cudaMemcpyHostToDevice);

	sum<<<4,256>>>(v1_d, v2_d, v3_d, N);

	cudaMemcpy(v3_h, v3_d, N * sizeof(int), cudaMemcpyDeviceToHost);


	for (int i = 0; i < N; ++i)
	{
		if (v3_h[i] != 1)
			printf("Error: %d %d\n", i, v3_h[i]);
	}

	cudaFree(v1_d);
	cudaFree(v2_d);
	cudaFree(v3_d);

	cudaDeviceSynchronize();
	return (0);
}
