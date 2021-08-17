//####################  Cabeçalho dos sensores e atuadores  ####################//
#ifndef DOUBLE_BUFFER_H
#define DOUBLE_BUFFER_H

#include <pthread.h>

#define TAMBUF 100

#define DBUFFER_MUTEX_INITIALIZER	PTHREAD_MUTEX_INITIALIZER
#define DBUFFER_COND_INITIALIZER	PTHREAD_COND_INITIALIZER
#define dbuffer_mutex_lock(mutex)	pthread_mutex_lock(mutex)
#define dbuffer_mutex_unlock(mutex)	pthread_mutex_unlock(mutex)
#define dbuffer_cond_signal(cond)	pthread_cond_signal(cond)
#define dbuffer_cond_wait(cond,mutex)	pthread_cond_wait(cond,mutex)
#define dbuffer_mutex_t			pthread_mutex_t
#define dbuffer_cond_t			pthread_cond_t

void escreve_buffer(double valor);

void libera_buffer();

double * acessa_buffer();

#endif
