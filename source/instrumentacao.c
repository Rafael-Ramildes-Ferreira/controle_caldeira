//####################  Implementação dos sensores e atuadores  ####################//
#include "instrumentacao.h"

#include <stdio.h>
#include <stdlib.h>
#include "comunicacao.h"


#define toFloat(x) atof(x+3)
#define limitado(val,inf,sup) ((val<=inf)?inf:(val>=sup)?sup:val)


/* Funções para manipular as estruturas  */
double le_sensor(struct sensor *x)  
{					
	instrumentacao_mutex_lock(&x->mutex);
	envia_armazena(x->code,x->valor);
	double val = toFloat(x->valor);
	instrumentacao_mutex_unlock(&x->mutex);

	return val;
}

void aciona_atuador(struct atuador *x,double valor)
{
	char msg[1000];
	
	instrumentacao_mutex_lock(&x->mutex);
	sprintf(msg,"%s%lf",x->code,limitado(valor,x->min,x->max));
	envia_armazena(msg,x->valor);
	instrumentacao_mutex_unlock(&x->mutex);

}

double le_atuador(struct atuador *x)
{
	instrumentacao_mutex_lock(&x->mutex);
	double val = toFloat(x->valor);
	instrumentacao_mutex_unlock(&x->mutex);

	return val;
}

double define_referencia(struct referencia *x,double val)
{
	instrumentacao_mutex_lock(&x->mutex);
	*(&x->valor) = val;
	instrumentacao_mutex_unlock(&x->mutex);
}

double le_referencia(struct referencia *x)
{
	instrumentacao_mutex_lock(&x->mutex);
	double val = x->valor;
	instrumentacao_mutex_unlock(&x->mutex);

	return val;
}
