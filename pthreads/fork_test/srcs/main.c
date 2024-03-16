#include "hello.h"

int	func(void)
{
	return (1); // B8 01 00 00 00
}

int	main(void)
{
	pid_t	pid;
	int		x;
	int		status;

	if (mprotect((void *)((long)&func & 0xfffffffffffff000), 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC) == -1)
	{
		perror("mprotect");
		return (1);
	}

	pid = fork();
	if (pid) // father
	{
		printf("father func:\t%p\n", &func);

		asm volatile ("decb (0x4011ff)\n"); // &func + 9

		wait(&status);

		printf("father post func:\t%p\n", &func);
		x = func();
		printf("return father:\t%d\n", x);
	}
	else // son
	{
		printf("son func:\t%p\n", &func);

		sleep(1);

		printf("son post func:\t%p\n", &func);
		x = func();
		printf("return son:\t%d\n", x);
	}

	return (0);
}
