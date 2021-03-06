//####################  Comunicação udp  ####################//
#include "comunicacao.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>


#define	TAM_MEU_BUFFER	1000
#define FALHA 1

/*  Variáveis da comunicação via rede  */
int porta_destino;
int socket_local;
struct sockaddr_in endereco_destino;
pthread_mutex_t mutex_udp = PTHREAD_MUTEX_INITIALIZER;


/*----------  Funções da comunicação UDP  ----------*/
int cria_socket_local(void)
{
	int socket_local;		/* Socket usado na comunicacão */

	socket_local = socket( PF_INET, SOCK_DGRAM, 0);
	if (socket_local < 0) {
		perror("socket");
		return -1;
	}
	return socket_local;
}


struct sockaddr_in cria_endereco_destino(char *destino, int porta_destino)
{
	struct sockaddr_in servidor; 	/* Endereço do servidor incluindo ip e porta */
	struct hostent *dest_internet;	/* Endereço destino em formato próprio */
	struct in_addr dest_ip;		/* Endereço destino em formato ip numérico */

	if (inet_aton ( destino, &dest_ip ))
		dest_internet = gethostbyaddr((char *)&dest_ip, sizeof(dest_ip), AF_INET);
	else
		dest_internet = gethostbyname(destino);

	if (dest_internet == NULL) {
		fprintf(stderr,"Endereço de rede inválido\n");
		exit(FALHA);
	}

	memset((char *) &servidor, 0, sizeof(servidor));
	memcpy(&servidor.sin_addr, dest_internet->h_addr_list[0], sizeof(servidor.sin_addr));
	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(porta_destino);

	return servidor;
}


void envia_mensagem(int socket_local, struct sockaddr_in endereco_destino, char *mensagem)
{
	/* Envia msg ao servidor */

	if (sendto(socket_local, mensagem, strlen(mensagem)+1, 0, (struct sockaddr *) &endereco_destino, sizeof(endereco_destino)) < 0 )
	{
		perror("sendto");
		return;
	}
}


int recebe_mensagem(int socket_local, char *buffer, int TAM_BUFFER)
{
	int bytes_recebidos;		/* Número de bytes recebidos */

	/* Espera pela msg de resposta do servidor */
	bytes_recebidos = recvfrom(socket_local, buffer, TAM_BUFFER, 0, NULL, 0);
	if (bytes_recebidos < 0)
	{
		perror("recvfrom");
	}

	return bytes_recebidos;
}


void envia_armazena(char msg[1000],char store[1000])
{
	int nrec;
	
	pthread_mutex_lock(&mutex_udp);						// --- Bloqueia

	envia_mensagem(socket_local, endereco_destino, msg);
	nrec = recebe_mensagem(socket_local, store, 1000);
	store[nrec] = '\0';

	pthread_mutex_unlock(&mutex_udp);					// --- Desloqueia
}

void inicializa_comunicacao(char *argv[3])
{
	porta_destino = atoi( argv[2]);
	socket_local = cria_socket_local();
	endereco_destino = cria_endereco_destino(argv[1], porta_destino);
}
