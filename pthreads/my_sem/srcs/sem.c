#include "hello.h"

int sem_init(t_sem *sem, int shared, unsigned int init_val)
{
  sem->counter = init_val;
  pthread_mutex_init(&sem->mutex, NULL);
  pthread_cond_init(&sem->cond, NULL);
  (void)shared;
  return (0);
}

int sem_wait(t_sem *sem)
{
  pthread_mutex_lock(&sem->mutex);
  if (sem->counter > 0)
    sem->counter--;
  else
  {
    sem->queue++;
    while (pthread_cond_wait(&sem->cond, &sem->mutex) != 0)
      ;
    sem->queue--;
  }
  pthread_mutex_unlock(&sem->mutex);
  return (0);
}

int sem_post(t_sem *sem)
{
  pthread_mutex_lock(&sem->mutex);
  if (sem->queue > 0)
    pthread_cond_signal(&sem->cond);
  else
    sem->counter++;
  pthread_mutex_unlock(&sem->mutex);
  return (0);
}

int sem_destroy(t_sem *sem);
{
  pthread_cond_destroy(&sem->cond);
  pthread_mutex_destroy(&sem->mutex);
  return (0);
}

