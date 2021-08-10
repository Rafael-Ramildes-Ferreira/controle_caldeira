//####################  Display do terminal  ####################//
#include <math.h>
#include <stdio.h>
#include <gconio.h>
#include <pthread.h>
#include "instrumentacao.h"


#define ESC "\033"

pthread_mutex_t mutex_scr = PTHREAD_MUTEX_INITIALIZER;


/*-----------  Funções de impressão de saída  ----------*/
void atualiza_valores_da_tela(struct atuador *lista1[],int lenght1,struct sensor *lista2[],int lenght2,int index)
{	
	pthread_mutex_lock(&mutex_scr);	
	printf("%s", ESC "[?25l");			// Cursor invisível

	/*  Atualiza atuadores  */
	printf("%s", ESC "[8A"); 			// Sobe 8 linhas
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

	printf("\n\n");
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

	printf("Escreva T 123.45 ou H 123.45 para redefinir o set-point\nde T ou H para 123.45 ou qualquer outro valor\n");
	printf("==>");
	printf("%s", ESC "[?25h");	// Cursor visível
	pthread_mutex_unlock(&mutex_scr);	
}

void print_warning(int valor)
{
	pthread_mutex_lock(&mutex_scr);
	printf("%s", ESC "[?25l");	// Cursor invisível
	printf("%s", ESC "[10A");	// Sobe 10 linhas
	printf("%s", ESC "[1m");	// Põe em negrito
	printf("%s", ESC "[38;5;196m");	// Põe em vermelho
	printf("%s", ESC "[K");		// Limpa a linha
	printf("#### WARNING: Temperatura Acima Do Limite Seguro: %d C. !!!!!!",valor);
	printf("%s", ESC "[0m");	// Reseta estilo da escrita
	printf("\n\n\n\n\n\n\n\n\n\n");	// Desce todas as 10 linhas
	printf("%s", ESC "[?25h");	// Cursor visível
	pthread_mutex_unlock(&mutex_scr);
}

void dont_print_warning()
{
	pthread_mutex_lock(&mutex_scr);
	printf("%s", ESC "[?25l");	// Cursor invisível
	printf("%s", ESC "[10A");	// Sobe 10 linhas
	printf("%s", ESC "[K");		// Limpa a linha
	printf("\n\n\n\n\n\n\n\n\n\n");	// Desce todas as 10 linhas
	printf("%s", ESC "[?25h");	// Cursor visível
	pthread_mutex_unlock(&mutex_scr);
}

void interpreta_texto(char input)
{
	pthread_mutex_lock(&mutex_scr);
	printf("%s", ESC "[4G");	// Vai para coluna 4
	printf("%s", ESC "[0K");	// Limpa a linha
	printf("%c", input);
	pthread_mutex_unlock(&mutex_scr);
}

double interpreta_numero(char input,int index,double val)
{
	pthread_mutex_lock(&mutex_scr);
	printf("%s[%dG", ESC, 6 + index);	// Vai para coluna 4
	printf("%s", ESC "[0K");		// Limpa a linha dali pra frente
	printf("%c", input);
	pthread_mutex_unlock(&mutex_scr);

	return 10*val + input - 48;		// Converte de char pra int e faz ser o número menos significativo de val
	
}

void comando_invalido()
{
	pthread_mutex_lock(&mutex_scr);
	printf("%s", ESC "[4G");
	printf("%s", ESC "[?25l");	// Cursor invisível
	printf("%s", ESC "[38;5;196m");	// Põe em vermelho
	printf("Comando invalido!!");
	printf("%s", ESC "[0m");	// Reseta estilo da escrita
	printf("%s", ESC "[?25h");
	pthread_mutex_unlock(&mutex_scr);

	delay(1500);

	pthread_mutex_lock(&mutex_scr);
	printf("%s", ESC "[4G");	// Vai para coluna 4
	printf("%s", ESC "[0K");	// Limpa a linha
	pthread_mutex_unlock(&mutex_scr);
}


/*-----------  Funções de interpretação de entrada  ----------*/
void interpreta_escrita(double *T, double *H)
{
	static int index;
	static double val;

	static int index_ponto;
	static int var;		// 1 -> T; 2 -> H

	char rec = getch();
	switch(rec){
		/*--------- Definem a variável ----------*/
		case 't':{
			interpreta_texto(rec-32);  // Em maiúsculo

			/*  Reseta parcialmente e define a variavel  */
			index_ponto = -1;
			index = 0;
			val = 0;
			var = 1;
		} break;
		case 'T':{
			interpreta_texto(rec);

			/*  Reseta parcialmente e define a variavel  */
			index_ponto = -1;
			index = 0;
			val = 0;
			var = 1;
		} break;
		case 'h':{
			interpreta_texto(rec-32);  // Em maiúsculo

			/*  Reseta parcialmente e define a variavel  */
			index_ponto = -1;
			index = 0;
			val = 0;
			var = 2;
		} break;
		case 'H':{
			interpreta_texto(rec);

			/*  Reseta parcialmente e define a variavel  */
			index_ponto = -1;
			index = 0;
			val = 0;
			var = 2;
		} break;

		/*--------- Inúteis ----------*/
		case ' ':{
		} break;

		/*--------- Pegam o valor ----------*/
		case '0':{
			val = interpreta_numero(rec,index,val);
			index++;
		}break;
		case '1':{
			val = interpreta_numero(rec,index,val);
			index++;
		}break;
		case '2':{
			val = interpreta_numero(rec,index,val);
			index++;
		}break;
		case '3':{
			val = interpreta_numero(rec,index,val);
			index++;
		}break;
		case '4':{
			val = interpreta_numero(rec,index,val);
			index++;
		}break;
		case '5':{
			val = interpreta_numero(rec,index,val);
			index++;
		}break;
		case '6':{
			val = interpreta_numero(rec,index,val);
			index++;
		}break;
		case '7':{
			val = interpreta_numero(rec,index,val);
			index++;
		}break;
		case '8':{
			val = interpreta_numero(rec,index,val);
			index++;
		}break;
		case '9':{
			val = interpreta_numero(rec,index,val);
			index++;
		}break;

		/*--------- Marca o ponto ----------*/
		case '.':{
			if(index_ponto < 0){
				printf("%s[%dG.", ESC, 6 + index);
				index++;
				index_ponto = index;
			}else{
				comando_invalido();

				/*  Reseta completamente  */
				index_ponto = -1;
				index = 0;
				val = 0;
				var = 0;
			}
		} break;
		case ',':{
			if(index_ponto < 0){
				printf("%s[%dG.", ESC, 6 + index);
				index++;
				index_ponto = index;
			}else{
				comando_invalido();

				/*  Reseta completamente  */
				index_ponto = -1;
				index = 0;
				val = 0;
				var = 0;
			}
		} break;

		/*--------- Termina a coleta ----------*/
		case '\n':{
			val *= pow(10,index_ponto-index);
			switch(var){
				case 1:{*T = val;}break;
				case 2:{*H = val;}break;
				default:{
					comando_invalido();
				}
			}

			/*  Limpa a tela  */
			printf("%s", ESC "[4G");	// Vai para coluna 4
			printf("%s", ESC "[0K");	// Limpa a linha

			/*  Reseta completamente  */
			index_ponto = -1;
			index = 0;
			val = 0;
			var = 0;
		} break;

		/*--------- Comando inválido ----------*/
		default:{
			comando_invalido();

			/*  Reseta completamente  */
			index_ponto = -1;
			index = 0;
			val = 0;
			var = 0;
		} break;
	}
}
