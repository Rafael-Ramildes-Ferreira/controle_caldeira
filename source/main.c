//####################  Inicia o programa  ####################//
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "./threads.h"
#include "./comunicacao.h"

#define FALHA 1


/*  Vari�veis da pthreads  */
pthread_t thread1,thread2,thread3,thread4,thread5;

	
/*  Dados  */
FILE *file;


int main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr,"Uso: NOME_DO_EXECUTAVEL endere�o porta \n");
		fprintf(stderr,"onde o endere�o � o endere�o do servidor \n");
		fprintf(stderr,"porta � o n�mero da porta do servidor \n");
		fprintf(stderr,"exemplo de uso:\n");
		fprintf(stderr,"   udpcliente baker.das.ufsc.br 1234 \n");
		exit(FALHA);
	}

	inicializa_comunicacao(argv);
	
	//FILE *file = fopen("./Dados/temp.txt","w");
	file = fopen("./dados/dados.csv","w");	// main.c est� em ./source, mas a posi��o � relativa a controladora (em ./)
	fprintf(file,"Temperatura desejada do tanque,Temperatura atual do tanque,Sinal de atuacao da temperatura,Nivel desejado do tanque,Nivel atual do Tanque,Sinal de atuacao da entrada dagua\n");


	pthread_create(&thread1,NULL,(void *) controla_temperatura,NULL);
	pthread_create(&thread2,NULL,(void *) controla_nivel,NULL);
	pthread_create(&thread3,NULL,(void *) imprime_dados,(void *) file);
	pthread_create(&thread4,NULL,(void *) monitora_temperatura,NULL);
	pthread_create(&thread5,NULL,(void *) le_teclado,NULL);


	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	pthread_join(thread3,NULL);
	pthread_join(thread4,NULL);
	pthread_join(thread5,NULL);
	
	fclose(file);

}
