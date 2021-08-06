//####################  Display do terminal  ####################//
#include <stdio.h>
#include <pthread.h>
#include "instrumentacao.h"


#define ESC "\033"

pthread_mutex_t mutex_scr = PTHREAD_MUTEX_INITIALIZER;

void atualiza_valores_da_tela(struct atuador *lista1[],int lenght1,struct sensor *lista2[],int lenght2,int index)
{	
	pthread_mutex_lock(&mutex_scr);	
	printf("%s", ESC "[?25l");			// Cursor invisível

	/*  Atualiza atuadores  */
	printf("%s", ESC "[6A"); 			// Sobe 6 linhas
	for(int i = 0;i < lenght1;i++){
		printf("%s", ESC "[7G"); 		// Anda 7 colunas para direita
		printf("%-8f", le_atuador(lista1[i]));	// Extrai e escreve o valor
		printf("\n");				// Passa pra próxima linha
	}

	/*  Atualiza sensores  */
	printf("%s", ESC "[4A");			// Sobe 4 linhas
	for(int i = 0;i < lenght2;i++){
		printf("%s", ESC "[30G");		// Anda 30 colunas para direita
		printf("%-8f",le_sensor(lista2[i]));	// Extrai e escreve o valor
		printf("\n");				// Passa pra próxima linha
	}
	printf("\n\n\n");

	/*  Atualiza tempo  */
	int min = index/60;
	printf("%s %02d:%02d\n", ESC "[6G",min, index - 60*min);

	printf("%s", ESC "[?25h");			// Cursor visível
	pthread_mutex_unlock(&mutex_scr);	
}

void inicializa_interface(struct atuador *lista1[],int lenght1,struct sensor *lista2[],int lenght2)
{
	pthread_mutex_lock(&mutex_scr);	
	printf("%s", ESC "[?25l");	// Cursor invisível
	printf("\n");

	printf("--------------------------------------\n");
	for(int i = 0;i < lenght1;i++){
		printf("%s = \n",lista1[i]->code);
	}
	printf("--------------------------------------\n");

	printf("%s", ESC "[5A");	// Sobe 5 linhas
	for(int i = 0;i < lenght2;i++){
		printf("%s%s = \n", ESC "[23G",lista2[i]->code);
	}

	printf("\n\n\n");			// Passa pra próxima linha
	printf("Tempo   :  \n");
	printf("%s", ESC "[?25h");	// Cursor visível
	pthread_mutex_unlock(&mutex_scr);	
}

void print_warning(int valor)
{
	pthread_mutex_lock(&mutex_scr);
	printf("%s", ESC "[?25l");	// Cursor invisível
	printf("%s", ESC "[8A");	// Sobe 8 linhas
	printf("%s", ESC "[1m");	// Põe em negrito
	printf("%s", ESC "[38;5;196m");	// Põe em vermelho
	printf("#### WARNING: Temperatura Acima Do Limite Seguro: %d C. !!!!!!",valor);
	printf("%s", ESC "[0m");	// Reseta estilo da escrita
	printf("\n\n\n\n\n\n\n\n");	// Desce todas as 8 linhas
	printf("%s", ESC "[?25h");	// Cursor visível
	pthread_mutex_unlock(&mutex_scr);
}

void dont_print_warning()
{
	pthread_mutex_lock(&mutex_scr);
	printf("%s", ESC "[?25l");	// Cursor invisível
	printf("%s", ESC "[8A");	// Sobe 8 linhas
	printf("%s", ESC "[K");		// Limpa a linha
	printf("\n\n\n\n\n\n\n\n");	// Desce todas as 8 linhas
	printf("%s", ESC "[?25h");	// Cursor visível
	pthread_mutex_unlock(&mutex_scr);
}
