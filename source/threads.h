//####################  Cabeçalho das Threads  ####################//
#ifndef THREADS_H
#define THREADS_H

#include <stdio.h>

/*  Threads do monitor  */
void imprime_dados();

void monitora_temperatura();

void salva_dados(FILE *file);

/*  Threads do controle  */
void controla_temperatura();

void controla_nivel();

void le_teclado();

#endif
