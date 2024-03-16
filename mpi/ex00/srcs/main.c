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
	
	if (my_rank != 0)
	{
		sprintf(greeting, "hello %d of %d from %s", my_rank, comm_sz, proc_name);
		MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}
	else
	{
		MPI_Status	status;
		int			count;
		for (int q = 1; q < comm_sz; ++q)
		{
			MPI_Recv(greeting, 200, MPI_CHAR, q, 0, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_CHAR, &count);
			printf("Recived: \"%s\" of %d bytes\n", greeting, count);
		}
	}
	MPI_Finalize();
	return (0);
}
