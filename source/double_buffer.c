//####################  Implementação do buffer duplo  ####################//
#include "double_buffer.h"

#include <errno.h>

#define TAMBUF 100

double buffer[2][TAMBUF];
int indice = 0;
int escrever = 0;
int ler =-1;

dbuffer_mutex_t buf_mutex = DBUFFER_MUTEX_INITIALIZER;
dbuffer_cond_t  buf_cheio = DBUFFER_COND_INITIALIZER;

int escreve_buffer(double valor)
{
	int buffer_completo = 0;

	dbuffer_mutex_lock(&buf_mutex);

	// Escreve no buffer
	buffer[escrever][indice++] = valor;

	// Verifica se o buffer ficou cheio
	if(indice==TAMBUF){
		libera_buffer();
		buffer_completo = 1;
	}

	dbuffer_mutex_unlock(&buf_mutex);
	
	return buffer_completo;
}

void libera_buffer()
{
	int is_mutex_required = 1;	// A função pode ser chamada de fora do escreve buffer, ai requer mutex
	if(dbuffer_mutex_trylock(&buf_mutex)==EBUSY)
		is_mutex_required = 0;

	ler = escrever;	// Informa que aleitura deve ser feita nesse buffer que estava sendo escrito
	indice = 0;	// E põe o índice da escrita no início
	escrever = (escrever + 1) % 2;		// Troca o buffer de escrita
	dbuffer_cond_signal(&buf_cheio);	// Sinaliza que o buffer está cheio

	if(is_mutex_required)
		dbuffer_mutex_unlock(&buf_mutex);
}

double * acessa_buffer(int* ultimo_buffer_lido)
{
	double *data;

	dbuffer_mutex_lock(&buf_mutex);

	// Libera o mutex e espera que a variável ler seja != -1
	while(ler==-1 || ler==*ultimo_buffer_lido){
		dbuffer_cond_wait(&buf_cheio,&buf_mutex);
	}

	data = buffer[ler];

	*ultimo_buffer_lido = ler;

	dbuffer_mutex_unlock(&buf_mutex);

	return data;
}

