//####################  Cabeçalho do Display  ####################//
#ifndef DISPLAY_H
#define DISPLAY_H

#include "instrumentacao.h"


void atualiza_valores_da_tela(struct atuador *lista1[],int lenght1,struct sensor *lista2[],int lenght2,int tempo);

void inicializa_interface(struct atuador *lista1[],int lenght1,struct sensor *lista2[],int lenght2);

void print_warning(int valor);

void dont_print_warning();

void finalizar_programa();

void interpreta_escrita(struct referencia *v[]);

#endif
