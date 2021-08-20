//####################  Implementação das Threads  ####################//
#include "threads.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "instrumentacao.h"
#include "double_buffer.h"
#include "display.h"
#define  DEFINE_VARS	// Deve-se definir as variáveis, não apenas declarar
#include "vars.h"

#define TEMPO_TOTAL 10

// Executa por uma hora (3600 s) então o loop ocorre 3600s/intervalo em segundos (espera intervalo em nano)
#define executa_nano(intervalo) for(int index = 0;index<TEMPO_TOTAL/(intervalo*1e-9);index++)
// Executa por uma hora (3600 s) então o loop ocorre 3600s/intervalo em segundos (espera intervalo em segundos)
#define executa_sec(index,intervalo) for(int index = 0;index<TEMPO_TOTAL/intervalo;index++)


/*  infos  */
float R = 0.001;		// 0.001 Grau/(J/s)
int S = 4184;			// 4184 J/KgC


/*-----------  Sequências de impressão  ----------*/
void imprime_dados()
{
	struct timespec time;
	int intervalo = 1;	  // 1s

	int n_atuadores = 4;
	struct atuador *atuadores_para_impressao[n_atuadores];
	atuadores_para_impressao[0] = &Q;
	atuadores_para_impressao[1] = &Ni;
	atuadores_para_impressao[2] = &Na;
	atuadores_para_impressao[3] = &Nf;

	int n_sensores = 2;
	struct sensor *sensoores_para_impressao[n_sensores];
	sensoores_para_impressao[0] = &T;
	sensoores_para_impressao[1] = &H;

	/*  Imprime no terminal  */
	inicializa_interface();//atuadores_para_impressao,n_atuadores,sensoores_para_impressao,n_sensores);
	
	/*  Prepara o Timer  */
	clock_gettime(CLOCK_MONOTONIC,&time);
	time.tv_sec += intervalo;

	executa_sec(index,intervalo){
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,&time,NULL);

		/*  Atualiza o terminal  */
		atualiza_valores_da_tela(atuadores_para_impressao,n_atuadores,sensoores_para_impressao,n_sensores,index);


		/*  Ajeita o Timer  */
		time.tv_sec += intervalo;
	}

	finalizar_programa();
}

/*-----------  Sequências para leitura de teclado  ----------*/
void escuta_teclado()
{
	struct timespec time_init,time_now;
	clock_gettime(CLOCK_MONOTONIC,&time_init);
	clock_gettime(CLOCK_MONOTONIC,&time_now);


	while(time_now.tv_sec - time_init.tv_sec < TEMPO_TOTAL){
		interpreta_escrita((struct referencia *[]) {&Tref, &Href});
				
		
		clock_gettime(CLOCK_MONOTONIC,&time_now);
	}
}

/*-----------  Sequências de coleção de dados  ----------*/
void monitoramento()
{
	struct timespec time;
	int intervalo = 0.01e+9;	// 0.1s = 10ms = 10 000 000ns
	
	int limite_seguro = 30;		// 30C

	int indice = 0;

	/*  Prepara o Timer  */
	clock_gettime(CLOCK_MONOTONIC,&time);
	time.tv_nsec += intervalo;

	executa_nano(intervalo){
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,&time,NULL);
		
		if(le_sensor(&T) > limite_seguro)
			print_warning(limite_seguro);
		else dont_print_warning();

		escreve_buffer((double) (indice++)*intervalo*1e-9);
		escreve_buffer(le_referencia(&Tref));
		escreve_buffer(le_sensor(&T));
		escreve_buffer(le_referencia(&Href));
		escreve_buffer(le_sensor(&H));

		/*  Ajeita o Timer  */
		time.tv_nsec += intervalo;
		while (time.tv_nsec >= 1e+9) {
		       time.tv_nsec -= 1e+9;
		       time.tv_sec++;
		}
	}
	libera_buffer();
}

void salva_dados(FILE *file)
{
	struct timespec time_init,time_now;
	clock_gettime(CLOCK_MONOTONIC,&time_init);

	double * dados;
	int ultimo_buffer_lido = -1;
	double sec;
	int min;

	do{
		dados = acessa_buffer(&ultimo_buffer_lido);
		
		for(int i = 0; i < TAMBUF;i+=5){
			sec = (*(dados+i));
			min = sec/60;
			fprintf(file,"%02d:%05.2f,%lf,%lf,%lf,%lf\n",
				min,			// Imprime o tempo em min:sec
				sec-min*60,		// segundos
				*(dados+i+1),		// Temperatude de Referência
				*(dados+i+2),		// Temperatura efetiva
				*(dados+i+3),		// Nível de referência
				*(dados+i+4));		// Nível efetivo
		}

		clock_gettime(CLOCK_MONOTONIC,&time_now);
	}while(time_now.tv_sec - time_init.tv_sec < TEMPO_TOTAL);
}


/*-----------  Sequências de controle  ----------*/
void controla_temperatura()
{
	struct timespec time;
	int intervalo = 0.05e+9;	// 0.05s = 50ms = 50 000 000ns

	/*  Variáveis do controle  */
	// Parâmetros
	double kp = 300000;		// Ganho proporcional
	double u;			// Atuação

	// Variáveis
	double erro;

	double tanque_auxiliar;

	/*  Prepara o relógio  */
	clock_gettime(CLOCK_MONOTONIC,&time);
	time.tv_nsec += intervalo;


	executa_nano(intervalo){
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,&time,NULL);
		
		/*  Define a atuação  */
		erro = le_referencia(&Tref) - le_sensor(&T);
		u = kp*(erro) + (le_sensor(&T)-le_sensor(&Ta))/R - S*(le_sensor(&Ti)-le_sensor(&T))*le_atuador(&Ni);

		/*  Envia a mensagem  */
		aciona_atuador(&Q,u);
		aciona_atuador(&Na,(u - le_atuador(&Q))/(S*(80-le_sensor(&T))));

		//printf("Controle da temperatura\n");

		/*  Ajeita o Timer  */
		time.tv_nsec += intervalo;
		while (time.tv_nsec >= 1e+9) {
		       time.tv_nsec -= 1e+9;
		       time.tv_sec++;
		}
	}
}


void controla_nivel()
{
	struct timespec time;
	int intervalo = 0.07e+9; 	  // 0.07s = 70ms = 70 000 000ns

	/*  Variáveis de controle  */
	// Parâmetros
	double kp = 50;
	double u;			// Atuação

	// Variável
	double erro;


	/*  Prepara o relógio  */
	clock_gettime(CLOCK_MONOTONIC,&time);
	time.tv_nsec += intervalo;


	executa_nano(intervalo){
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,&time,NULL);

		/*  Define a atuação  */
		erro = le_referencia(&Href) - le_sensor(&H);
		u = kp*erro + le_sensor(&No) - le_atuador(&Na);
		
		/*  Envia mensagem  */
		aciona_atuador(&Ni,u);
		aciona_atuador(&Nf,-u);
		
		//printf("Controle do nível\n");

		/*  Ajeita o Timer  */
		time.tv_nsec += intervalo;
		while (time.tv_nsec >= 1e+9) {
		       time.tv_nsec -= 1e+9;
		       time.tv_sec++;
		}
	}
}

