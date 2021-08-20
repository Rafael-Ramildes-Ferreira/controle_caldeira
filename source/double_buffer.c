//####################  Implementação do buffer duplo  ####################//
#include "double_buffer.h"

#define TAMBUF 100

double buffer[2][TAMBUF];
int indice = 0;
int escrever = 0;
int ler =-1;

dbuffer_mutex_t buf_mutex = DBUFFER_MUTEX_INITIALIZER;
dbuffer_cond_t  buf_cheio = DBUFFER_COND_INITIALIZER;

void escreve_buffer(double valor)
{
	dbuffer_mutex_lock(&buf_mutex);

	// Escreve no buffer
	buffer[escrever][indice++] = valor;

	// Verifica se o buffer ficou cheio
	if(indice==TAMBUF)
		libera_buffer();

	dbuffer_mutex_unlock(&buf_mutex);
}

void libera_buffer()
{
	ler = escrever;	// Informa que aleitura deve ser feita nesse buffer que estava sendo escrito
	indice = 0;	// E põe o índice da escrita no início
	escrever = (escrever + 1) % 2;		// Troca o buffer de escrita
	dbuffer_cond_signal(&buf_cheio);	// Sinaliza que o buffer está cheio
}

double * acessa_buffer(int* ultimo_buffer_lido)
{
	double *data;

	dbuffer_mutex_lock(&buf_mutex);

	// Libera o mutex e espera que a variável ler seja != -1
	while(ler==-1 || ler==*ultimo_buffer_lido)
		dbuffer_cond_wait(&buf_cheio,&buf_mutex);

	data = buffer[ler];

	*ultimo_buffer_lido = ler;

	dbuffer_mutex_unlock(&buf_mutex);

	return data;
}

