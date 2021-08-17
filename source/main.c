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


/*  Variáveis da pthreads  */
pthread_t thread1,thread2,thread3,thread4,thread5,thread6;

	
/*  Dados  */
FILE *file;


int main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr,"Uso: NOME_DO_EXECUTAVEL endereço porta \n");
		fprintf(stderr,"onde o endereço é o endereço do servidor \n");
		fprintf(stderr,"porta é o número da porta do servidor \n");
		fprintf(stderr,"exemplo de uso:\n");
		fprintf(stderr,"   udpcliente baker.das.ufsc.br 1234 \n");
		exit(FALHA);
	}

	inicializa_comunicacao(argv);
	
	//FILE *file = fopen("./Dados/temp.txt","w");
	file = fopen("./dados/dados.csv","w");	// main.c está em ./source, mas a posição é relativa a controladora (em ./)
	fprintf(file,"Tempo em min:sec,Temperatura desejada do tanque,Temperatura atual do tanque,Nivel desejado do tanque,Nivel atual do Tanque\n");


	pthread_create(&thread1,NULL,(void *) controla_temperatura,NULL);
	pthread_create(&thread2,NULL,(void *) controla_nivel,NULL);
	pthread_create(&thread3,NULL,(void *) imprime_dados,NULL);//(void *) file);
	pthread_create(&thread4,NULL,(void *) monitora_temperatura,NULL);
	pthread_create(&thread5,NULL,(void *) le_teclado,NULL);
	pthread_create(&thread6,NULL,(void *) salva_dados,(void *) file);


	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	pthread_join(thread3,NULL);
	pthread_join(thread4,NULL);
	pthread_join(thread5,NULL);
	pthread_join(thread6,NULL);
	
	printf("%s","\033[?25h");	// Cursor visível
	fclose(file);

}
