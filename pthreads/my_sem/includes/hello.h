#ifndef HELLO_H
# define HELLO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>

typdef struct s_sem
{
  usigned int     counter;
  usigned int     queue;
  pthread_cond_t  cond;
  pthread_mutex_t mutex;
} t_sem;

int sem_init(t_sem *sem, int shared, unsigned int init_val);
int sem_wait(t_sem *sem);
int sem_post(t_sem *sem);
int sem_destroy(t_sem *sem);

#endif
