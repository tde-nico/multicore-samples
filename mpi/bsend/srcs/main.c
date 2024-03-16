#include "hello.h"

int	main(void)
{
	int num, rank, i;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num);

	if (rank == 0)
	{
		unsigned char *buff = malloc(sizeof(unsigned char) * SIZE);
		MPI_Buffer_attach(buff, SIZE);
		char *msg = "test msg";
		for (i = 1; i < num; ++i)
			MPI_Bsend(msg, strlen(msg) + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
		unsigned char *bptr;
		int bsize;
		MPI_Buffer_detach(&bptr, &bsize);
		free(bptr);
	}
	else
	{
		//MPI_Status status;
		char msg[SIZE];
		MPI_Recv(msg, SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("%s\n", msg);
	}
	MPI_Finalize();
	return (0);
}
