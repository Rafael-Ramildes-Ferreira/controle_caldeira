//####################  Cabeçalho dos sensores e atuadores  ####################//

#ifndef instrumentos
#define instrumentos

struct atuador
{
	char code[3];
	//double valor;
	pthread_mutex_t mutex;
	char valor[1000];
};

struct sensor
{
	char code[4];
	pthread_mutex_t mutex;
	char valor[1000];
};

#endif

double le_sensor(struct sensor *x);

void aciona_atuador(struct atuador *x,double valor);

double le_atuador(struct atuador *x);
