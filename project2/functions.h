/*
 * File:  functions.c
 * Author:  Francisco Sousa
 * Description: Ficheiro de cabecalho de functions.c
*/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "hashtablee.h"
#include "hashtablej.h"
#include "structs.h"


void add_jogo(HashtableJ table_jogos, HashtableE table_equipas, list* lista, int nl);
void add_equipa(HashtableE table_equipas, int nl);
void lista_jogos(const list* list, int nl);
void remove_jogo(HashtableJ table_jogo, HashtableE table_equipa, list* lista, int nl);
void procura_jogo(HashtableJ table_jogo, int nl);
void procura_equipa(HashtableE table_equipa, int nl);
void altera_score(HashtableJ table_jogo, HashtableE table_equipa, int nl);
void mais_ganhos(HashtableE equipas, int nl);

#endif