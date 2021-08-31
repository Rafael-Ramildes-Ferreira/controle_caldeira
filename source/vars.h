//####################  Cabeçalho de criação de variáveis  ####################//
#ifndef VARS_H
#define VARS_H

#include "instrumentacao.h"

#ifndef DEFINE_VARS
// Apenas declara as variáveis (não aloca memória)
#define _DECL				extern
#define _init_atuador(token,max,min)
#define _init_sensor(token)
#define _init_referencia(init)

#else
// Define as variáveis (aloca memória)
#define _DECL
#define _init_atuador(token,max,min)	= {token,max,min,INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"};
#define _init_sensor(token)		= {token,INSTRUMENTACAO_MUTEX_INITIALIZER,"0000"};
#define _init_referencia(init)		= {init, INSTRUMENTACAO_MUTEX_INITIALIZER};

#endif


/*  Set points  */
_DECL struct referencia Tref _init_referencia(30);
_DECL struct referencia Href _init_referencia(1.5);

/*  inicializa_atuadores  */
_DECL struct atuador Q  _init_atuador("aq-",1000000,0);
_DECL struct atuador Ni _init_atuador("ani",100,0);
_DECL struct atuador Nf _init_atuador("anf",100,0);
_DECL struct atuador Na _init_atuador("ana",10,0);

/*  inicializa_sensores  */
_DECL struct sensor T  _init_sensor("st-0"); 
_DECL struct sensor H  _init_sensor("sh-0"); 
_DECL struct sensor No _init_sensor("sno0"); 
_DECL struct sensor Ta _init_sensor("sta0"); 
_DECL struct sensor Ti _init_sensor("sti0"); 

#endif
