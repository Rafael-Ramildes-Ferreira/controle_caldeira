//####################  Implementação dos sensores e atuadores  ####################//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "comunicacao.h"
#include "instrumentacao.h"


#define toFloat(x) atof(x+3)

/*  Definição das estruturas  */
struct atuador;/*
{
	char code[3];
	//double valor;
	pthread_mutex_t mutex;
	char valor[1000];
};*/

struct sensor;/*
{
	char code[4];
	pthread_mutex_t mutex;
	char valor[1000];
};*/


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
