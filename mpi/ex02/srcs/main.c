#include "hello.h"

int	main(void)
{
	int		comm_sz;
	int		my_rank;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	char	proc_name[200];
	char	greeting[300];
	int		name_len;

	MPI_Get_processor_name(proc_name, &name_len);

	MPI_Win	win;
	sprintf(greeting, "hello %d of %d from %s", my_rank, comm_sz, proc_name);

	float	frank = (float)my_rank;
	float	*buf = NULL;
	MPI_Win_allocate(sizeof(float), sizeof(float), MPI_INFO_NULL, MPI_COMM_WORLD, &buf, &win);

	MPI_Win_fence(0, win);
	if (my_rank != 0)
	{
		MPI_Accumulate(&frank, sizeof(float), MPI_FLOAT, 0, 0, sizeof(float), MPI_FLOAT, MPI_SUM, win);
		MPI_Win_fence(0, win);
	}
	else
	{
		MPI_Win_fence(0, win);
		for (int i = 0; i < (comm_sz - 1); ++i)
		{
			printf("Recived: %f %ld\n", *buf);
		}
	}

	MPI_Win_free(&win);
	MPI_Finalize();
	return (0);
}
