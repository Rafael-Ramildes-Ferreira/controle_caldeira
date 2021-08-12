//####################  Implementação dos sensores e atuadores  ####################//
#include "instrumentacao.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "comunicacao.h"


#define toFloat(x) atof(x+3)


/* Funções para manipular as estruturas  */
double le_sensor(struct sensor *x)  
{					
	pthread_mutex_lock(&x->mutex);
	envia_armazena(x->code,x->valor);
	double val = toFloat(x->valor);
	pthread_mutex_unlock(&x->mutex);

	return val;
}

void aciona_atuador(struct atuador *x,double valor)
{
	char msg[1000];

	pthread_mutex_lock(&x->mutex);
	sprintf(msg,"%s%f",x->code,valor);
	envia_armazena(msg,x->valor);
	pthread_mutex_unlock(&x->mutex);

}

double le_atuador(struct atuador *x)
{
	pthread_mutex_lock(&x->mutex);
	double val = toFloat(x->valor);
	pthread_mutex_unlock(&x->mutex);

	return val;
}

double define_referencia(struct referencia *x,double val)
{
	pthread_mutex_lock(&x->mutex);
	*(&x->valor) = val;
	pthread_mutex_unlock(&x->mutex);
}

double le_referencia(struct referencia *x)
{
	pthread_mutex_lock(&x->mutex);
	double val = x->valor;
	pthread_mutex_unlock(&x->mutex);

	return val;
}
