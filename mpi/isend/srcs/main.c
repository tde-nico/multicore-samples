#include "hello.h"

int	main(void)
{
	char	greeting[MAX_STRING];
	int		comm_sz;
	int		my_rank;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	if (my_rank)
	{
		sprintf(greeting, GREET, my_rank, comm_sz);
		MPI_Request req;
		MPI_Isend(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &req);
		printf("Waiting [%d]\n", my_rank);
		MPI_Status s;
		int	flag;
		MPI_Test(&req, &flag, &s); // non blocking
		MPI_Wait(&req, &s); // blocking
		printf("done [%d] %d\n", my_rank, flag);
	}
	else
	{
		printf(GREET, my_rank, comm_sz);
		for (int q = 1; q < comm_sz; ++q)
		{
			MPI_Request req;
			MPI_Irecv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, &req);
			printf("%s", greeting);
			printf("Waiting\n");
			MPI_Status s;
			int	flag;
			MPI_Test(&req, &flag, &s); // non blocking
			MPI_Wait(&req, &s); // blocking
			printf("done %d - %d\n", q, flag);
		}
	}
	MPI_Finalize();
	return (0);
}
