/*  Programa de demonstra��o de uso de sockets UDP em C no Linux
 *  Funcionamento:
 *  O programa cliente envia uma msg para o servidor. Essa msg � uma palavra.
 *  O servidor envia outra palavra como resposta.
 */

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

#define FALHA 1

#define	TAM_MEU_BUFFER	1000




int cria_socket_local(void)
{
	int socket_local;		/* Socket usado na comunicac�o */

	socket_local = socket( PF_INET, SOCK_DGRAM, 0);
	if (socket_local < 0) {
		perror("socket");
		return -1;
	}
	return socket_local;
}





struct sockaddr_in cria_endereco_destino(char *destino, int porta_destino)
{
	struct sockaddr_in servidor; 	/* Endere�o do servidor incluindo ip e porta */
	struct hostent *dest_internet;	/* Endere�o destino em formato pr�prio */
	struct in_addr dest_ip;		/* Endere�o destino em formato ip num�rico */

	if (inet_aton ( destino, &dest_ip ))
		dest_internet = gethostbyaddr((char *)&dest_ip, sizeof(dest_ip), AF_INET);
	else
		dest_internet = gethostbyname(destino);

	if (dest_internet == NULL) {
		fprintf(stderr,"Endere�o de rede inv�lido\n");
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
	int bytes_recebidos;		/* N�mero de bytes recebidos */

	/* Espera pela msg de resposta do servidor */
	bytes_recebidos = recvfrom(socket_local, buffer, TAM_BUFFER, 0, NULL, 0);
	if (bytes_recebidos < 0)
	{
		perror("recvfrom");
	}

	return bytes_recebidos;
}





int main(int argc, char *argv[])
{
	if (argc < 4) {
		fprintf(stderr,"Uso: udpcliente endere�o porta palavra \n");
		fprintf(stderr,"onde o endere�o � o endere�o do servidor \n");
		fprintf(stderr,"porta � o n�mero da porta do servidor \n");
		fprintf(stderr,"palavra � a palavra que ser� enviada ao servidor \n");
		fprintf(stderr,"exemplo de uso:\n");
		fprintf(stderr,"   udpcliente baker.das.ufsc.br 1234 \"ola\"\n");
		exit(FALHA);
	}

	int porta_destino = atoi( argv[2]);

	int socket_local = cria_socket_local();

	struct sockaddr_in endereco_destino = cria_endereco_destino(argv[1], porta_destino);


	char msg_recebida[1000];
	int nrec;

	printf("tentativa\n");

	envia_mensagem(socket_local, endereco_destino, argv[3]);

	nrec = recebe_mensagem(socket_local, msg_recebida, 1000);
	
	msg_recebida[nrec] = '\0';
	
	printf("Mensagem de resposta com %d bytes >>>%s\n", nrec, msg_recebida);



}
