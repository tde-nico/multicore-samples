#include "hello.h"

#define N (1<<11)
//#define N (1<<16)

__global__ void	reduct(int *v, int *r)
{
	int	id = threadIdx.x + blockIdx.x * blockDim.x;
	int	stride = N / 2;

	for (int i = stride; i > 0; i >>= 1)
	{
		if (id < i)
			v[id] += v[id + i];
		__syncthreads();
	}
	if (!id)
		*r = v[0];
}

int	main(void)
{
	int	v1_h[N];
	int	res_h;
	int	*v1_d;
	int	*res_d;
	for (int i = 0; i < N; ++i)
	{
		v1_h[i] = 3;
	}


	cudaMalloc((void **)&v1_d, N * sizeof(int));
	cudaMalloc((void **)&res_d, sizeof(int));

	cudaMemcpy(v1_d, v1_h, N * sizeof(int), cudaMemcpyHostToDevice);

	reduct<<< 1, 1024 >>>(v1_d, res_d);
	//reduct<<< 32, 1024 >>>(v1_d, res_d);

	cudaMemcpy(&res_h, res_d, sizeof(int), cudaMemcpyDeviceToHost);

	printf("res = %d, expected = %d\n", res_h, N * 3);

	cudaFree(v1_d);
	cudaFree(res_d);

	cudaDeviceSynchronize();
	return (0);
}
