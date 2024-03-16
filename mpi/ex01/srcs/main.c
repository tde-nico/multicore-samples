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

	char	*buf = NULL;
	MPI_Win_allocate(300 * (comm_sz - 1), 300, MPI_INFO_NULL, MPI_COMM_WORLD, &buf, &win);
	
	MPI_Win_fence(0, win);

	if (my_rank != 0)
	{
		sprintf(greeting, "hello %d of %d from %s", my_rank, comm_sz, proc_name);
		MPI_Put(greeting, strlen(greeting)+1, MPI_CHAR, 0, my_rank-1, strlen(greeting)+1, MPI_CHAR, win);
		MPI_Win_fence(0, win);
	}
	else
	{
		MPI_Win_fence(0, win);
		for (int i = 0; i < (comm_sz - 1); ++i)
		{
			printf("Recived: \"%s\" of %ld bytes\n", buf + 300 * i, strlen(buf + 300 * i)+1);
		}
	}

	MPI_Win_free(&win);
	MPI_Finalize();
	return (0);
}
