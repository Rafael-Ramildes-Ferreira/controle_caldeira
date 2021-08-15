//####################  Implementação das Threads  ####################//
#include "threads.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "instrumentacao.h"
#include "double_buffer.h"
#include "display.h"

#define TEMPO_TOTAL 3600

// Executa por uma hora (3600 s) então o loop ocorre 3600s/intervalo em segundos (espera intervalo em nano)
#define executa_nano(intervalo) for(int index = 0;index<TEMPO_TOTAL/(intervalo*1e-9);index++)
// Executa por uma hora (3600 s) então o loop ocorre 3600s/intervalo em segundos (espera intervalo em segundos)
#define executa_sec(index,intervalo) for(int index = 0;index<TEMPO_TOTAL/intervalo;index++)


/*  infos  */
float R = 0.001;		// 0.001 Grau/(J/s)
int S = 4184;			// 4184 J/KgC


/*  Set points  */
struct referencia Tref = {30, INSTRUMENTACAO_MUTEX_INITIALIZER};
struct referencia Href = {1.5,INSTRUMENTACAO_MUTEX_INITIALIZER};
/*
double Tref = 30;		// 30C
double Href = 1.5;		// 1.5 m*/

/*  inicializa_atuadores  */
struct atuador Q  = {"aq-",1000000,0,INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"};
struct atuador Ni = {"ani",100,0,INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"};
struct atuador Nf = {"anf",100,0,INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"};
struct atuador Na = {"ana",10,0,INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"};


/*  inicializa_sensores  */
struct sensor T  = {"st-0",INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"}; 
struct sensor H  = {"sh-0",INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"}; 
struct sensor No = {"sno0",INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"}; 
struct sensor Ta = {"sta0",INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"}; 
struct sensor Ti = {"sti0",INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"}; 


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

	/*  Prepara o Timer  */
	clock_gettime(CLOCK_MONOTONIC,&time);
	time.tv_sec += intervalo;

	/*  Imprime no terminal  */
	inicializa_interface(atuadores_para_impressao,n_atuadores,sensoores_para_impressao,n_sensores);
	
	executa_sec(index,intervalo){
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,&time,NULL);

		/*  Atualiza o terminal  */
		atualiza_valores_da_tela(atuadores_para_impressao,n_atuadores,sensoores_para_impressao,n_sensores,index);


		/*  Imprime no file  */
		//fprintf(file,"%f,%f,%f,%f,%f,%f\n",le_referencia(&Tref),le_sensor(&T),.00,le_referencia(&Href),le_sensor(&H),.00);

		/*  Ajeita o Timer  */
		time.tv_sec += intervalo;
	}

	finalizar_programa();
}

void monitora_temperatura()
{
	struct timespec time;
	int intervalo = 0.01e+9;	// 0.1s = 10ms = 10 000 000ns
	
	int limite_seguro = 30;		// 30C

	/*  Prepara o Timer  */
	clock_gettime(CLOCK_MONOTONIC,&time);
	time.tv_nsec += intervalo;

	executa_nano(intervalo){
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,&time,NULL);
		
		if(le_sensor(&T) > limite_seguro)
			print_warning(limite_seguro);
		else dont_print_warning();

		escreve_buffer(le_referencia(&Tref));
		escreve_buffer(le_sensor(&T));
		escreve_buffer(0.0);
		escreve_buffer(le_referencia(&Href));
		escreve_buffer(le_sensor(&H));
		escreve_buffer(0.0);

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

	do{
		dados = acessa_buffer();
		
		for(int i = 0; i < TAMBUF;i++)
			fprintf(file,"%f%c",*(dados+i),((i+1)%6==0)?'\n':',');
							// Quebra linha ou põe ','

		clock_gettime(CLOCK_MONOTONIC,&time_now);
	}while(time_now.tv_sec - time_init.tv_sec < TEMPO_TOTAL);
}

/*-----------  Sequências para leitura de teclado  ----------*/
void le_teclado()
{
	struct timespec time_init,time_now;
	clock_gettime(CLOCK_MONOTONIC,&time_init);
	clock_gettime(CLOCK_MONOTONIC,&time_now);


	while(time_now.tv_sec - time_init.tv_sec < TEMPO_TOTAL){
		interpreta_escrita((struct referencia *[]) {&Tref, &Href});
				
		
		clock_gettime(CLOCK_MONOTONIC,&time_now);
	}
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
		if(u<0) {
			aciona_atuador(&Nf,-u);
			aciona_atuador(&Ni,0);
		} else {
			aciona_atuador(&Ni,u);
			aciona_atuador(&Nf,0);
		}
		
		//printf("Controle do nível\n");

		/*  Ajeita o Timer  */
		time.tv_nsec += intervalo;
		while (time.tv_nsec >= 1e+9) {
		       time.tv_nsec -= 1e+9;
		       time.tv_sec++;
		}
	}
	
}
