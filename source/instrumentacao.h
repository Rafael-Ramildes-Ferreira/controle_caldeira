//####################  Cabeçalho dos sensores e atuadores  ####################//
#ifndef INSTRUMENTACAO_H
#define INSTRUMENTACAO_H

#include <pthread.h>

#define INSTRUMENTACAO_MUTEX_INITIALIZER	PTHREAD_MUTEX_INITIALIZER
#define instrumentacao_mutex_lock(mutex) 	pthread_mutex_lock(mutex)
#define instrumentacao_mutex_unlock(mutex) 	pthread_mutex_unlock(mutex)
#define instrumentacao_mutex_t 			pthread_mutex_t


struct atuador
{
	char token[3];
	double max;
	double min;
	pthread_mutex_t mutex;
	char valor[1000];
};

struct sensor
{
	char token[4];
	pthread_mutex_t mutex;
	char valor[1000];
};

struct referencia
{
	double valor;
	pthread_mutex_t mutex;
};


/*  Set points  *//*
struct referencia Tref = {30, INSTRUMENTACAO_MUTEX_INITIALIZER};
struct referencia Href = {1.5,INSTRUMENTACAO_MUTEX_INITIALIZER};*/

/*  inicializa_atuadores  *//*
struct atuador Q  = {"aq-",1000000,0,INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"};
struct atuador Ni = {"ani",100,	   0,INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"};
struct atuador Nf = {"anf",100,    0,INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"};
struct atuador Na = {"ana",10,     0,INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"};*/

/*  inicializa_sensores  *//*
struct sensor T  = {"st-0",INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"}; 
struct sensor H  = {"sh-0",INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"}; 
struct sensor No = {"sno0",INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"}; 
struct sensor Ta = {"sta0",INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"}; 
struct sensor Ti = {"sti0",INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"}; */


double le_sensor(struct sensor *x);

void aciona_atuador(struct atuador *x,double valor);

double le_atuador(struct atuador *x);

double define_referencia(struct referencia *x,double val);

double le_referencia(struct referencia *x);

#endif
